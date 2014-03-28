/*
 *  Copyright (c) 2000 - 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Bumjin Im <bj.im@samsung.com>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

/**
 * @file    dbus-connection.cpp
 * @author  Piotr Bartosiewicz (p.bartosiewi@partner.samsung.com)
 * @brief   Dbus connection class
 */

#include "dbus-connection.hpp"
#include "dbus-exception.hpp"
#include "log.hpp"

namespace security_containers {
namespace dbus {

namespace {

const std::string SYSTEM_BUS_ADDRESS = "unix:path=/var/run/dbus/system_bus_socket";
const std::string INTROSPECT_INTERFACE = "org.freedesktop.DBus.Introspectable";
const std::string INTROSPECT_METHOD = "Introspect";

const int CALL_METHOD_TIMEOUT_MS = 1000;

template<class Callback>
void deleteCallback(gpointer data)
{
    delete reinterpret_cast<Callback*>(data);
}

class ScopedError {
public:
    ScopedError() : mError(NULL) {}
    ~ScopedError()
    {
        if (mError) {
            g_error_free(mError);
        }
    }
    void strip()
    {
        g_dbus_error_strip_remote_error(mError);
    }
    operator bool () const
    {
        return mError;
    }
    GError** operator& ()
    {
        return &mError;
    }
    const GError* operator->() const
    {
        return mError;
    }
    friend std::ostream& operator<<(std::ostream& os, const ScopedError& e)
    {
        os << e->message;
        return os;
    }
private:
    GError* mError;
};

class MethodResultBuilderImpl : public MethodResultBuilder {
public:
    MethodResultBuilderImpl(GDBusMethodInvocation* invocation)
        : mInvocation(invocation), mResultSet(false) {}
    void set(GVariant* parameters)
    {
        g_dbus_method_invocation_return_value(mInvocation, parameters);
        mResultSet = true;
    }
    void setVoid()
    {
        set(NULL);
    }
    void setError(const std::string& name, const std::string& message)
    {
        g_dbus_method_invocation_return_dbus_error(mInvocation, name.c_str(), message.c_str());
        mResultSet = true;
    }
    bool isUndefined() const
    {
        return !mResultSet;
    }
private:
    GDBusMethodInvocation* mInvocation;
    bool mResultSet;
};

void throwDbusException(const ScopedError& e)
{
    if (e->domain == g_io_error_quark()) {
        if (e->code == G_IO_ERROR_DBUS_ERROR) {
            throw DbusCustomException(e->message);
        } else {
            throw DbusIOException(e->message);
        }
    } else if (e->domain == g_dbus_error_quark()) {
        throw DbusOperationException(e->message);
    } else if (e->domain == g_markup_error_quark()) {
        throw DbusInvalidArgumentException(e->message);
    } else {
        throw DbusException(e->message);
    }
}

} // namespace

DbusConnection::Pointer DbusConnection::create(const std::string& address)
{
    return Pointer(new DbusConnection(address));
}

DbusConnection::Pointer DbusConnection::createSystem()
{
    return create(SYSTEM_BUS_ADDRESS);
}

DbusConnection::DbusConnection(const std::string& address)
    : mConnection(NULL)
    , mNameId(0)
{
    ScopedError error;
    const GDBusConnectionFlags flags =
        static_cast<GDBusConnectionFlags>(G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT |
                                          G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION);
    mConnection = g_dbus_connection_new_for_address_sync(address.c_str(),
                                                         flags,
                                                         NULL,
                                                         NULL,
                                                         &error);
    if (error) {
        error.strip();
        LOGE("Could not connect to " << address << "; " << error);
        throwDbusException(error);
    }
}

DbusConnection::~DbusConnection()
{
    if (mNameId) {
        g_bus_unown_name(mNameId);
    }
    //TODO should we unregister, flush, close etc?
    //if (!g_dbus_connection_close_sync(mConnection, NULL, NULL)) {
    //    LOGE("Could not close connection");
    //}
    g_object_unref(mConnection);
    LOGT("Connection deleted");
}

void DbusConnection::setName(const std::string& name,
                             const VoidCallback& onNameAcquired,
                             const VoidCallback& onNameLost)
{
    mNameId = g_bus_own_name_on_connection(mConnection,
                                           name.c_str(),
                                           G_BUS_NAME_OWNER_FLAGS_NONE,
                                           &DbusConnection::onNameAcquired,
                                           &DbusConnection::onNameLost,
                                           new NameCallbacks(onNameAcquired, onNameLost),
                                           &deleteCallback<NameCallbacks>);
}

void DbusConnection::onNameAcquired(GDBusConnection*, const gchar* name, gpointer userData)
{
    LOGD("Name acquired " << name);
    const NameCallbacks& callbacks = *reinterpret_cast<const NameCallbacks*>(userData);
    if (callbacks.nameAcquired) {
        callbacks.nameAcquired();
    }
}

void DbusConnection::onNameLost(GDBusConnection*, const gchar* name, gpointer userData)
{
    LOGE("Name lost " << name);
    const NameCallbacks& callbacks = *reinterpret_cast<const NameCallbacks*>(userData);
    if (callbacks.nameLost) {
        callbacks.nameLost();
    }
}

void DbusConnection::emitSignal(const std::string& objectPath,
                                const std::string& interface,
                                const std::string& name,
                                GVariant* parameters)
{
    ScopedError error;
    g_dbus_connection_emit_signal(mConnection,
                                  NULL,
                                  objectPath.c_str(),
                                  interface.c_str(),
                                  name.c_str(),
                                  parameters,
                                  &error);
    if (error) {
        error.strip();
        LOGE("Emit signal failed; " << error);
        throwDbusException(error);
    }
}

void DbusConnection::signalSubscribe()
{
    g_dbus_connection_signal_subscribe(mConnection,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL,
                                       G_DBUS_SIGNAL_FLAGS_NONE,
                                       &DbusConnection::onSignal,
                                       NULL,//data
                                       NULL);
}

void DbusConnection::onSignal(GDBusConnection*,
                              const gchar* sender,
                              const gchar* object,
                              const gchar* interface,
                              const gchar* name,
                              GVariant* /*parameters*/,
                              gpointer /*userData*/)
{
    LOGD("Signal: " << sender << "; " << object << "; " << interface << "; " << name);
    //TODO call some callback
}

std::string DbusConnection::introspect(const std::string& busName, const std::string& objectPath)
{
    GVariantPtr result = DbusConnection::callMethod(busName,
                                                    objectPath,
                                                    INTROSPECT_INTERFACE,
                                                    INTROSPECT_METHOD,
                                                    NULL,
                                                    "(s)");
    const gchar* s;
    g_variant_get(result.get(), "(&s)", &s);
    std::string xml = s;
    return xml;
}

void DbusConnection::registerObject(const std::string& objectPath,
                                    const std::string& objectDefinitionXml,
                                    const MethodCallCallback& callback)
{
    ScopedError error;
    GDBusNodeInfo* nodeInfo = g_dbus_node_info_new_for_xml(objectDefinitionXml.c_str(), &error);
    if (nodeInfo != NULL && (nodeInfo->interfaces == NULL ||
                             nodeInfo->interfaces[0] == NULL ||
                             nodeInfo->interfaces[1] != NULL)) {
        g_dbus_node_info_unref(nodeInfo);
        g_set_error(&error,
                    G_MARKUP_ERROR,
                    G_MARKUP_ERROR_INVALID_CONTENT,
                    "Expected exactly one interface");
    }
    if (error) {
        error.strip();
        LOGE("Invalid xml; " << error);
        throwDbusException(error);
    }
    GDBusInterfaceInfo* interfaceInfo = nodeInfo->interfaces[0];

    GDBusInterfaceVTable vtable;
    vtable.method_call = &DbusConnection::onMethodCall;
    vtable.get_property = NULL;
    vtable.set_property = NULL;

    g_dbus_connection_register_object(mConnection,
                                      objectPath.c_str(),
                                      interfaceInfo,
                                      &vtable,
                                      new MethodCallCallback(callback),
                                      &deleteCallback<MethodCallCallback>,
                                      &error);
    g_dbus_node_info_unref(nodeInfo);
    if (error) {
        error.strip();
        LOGE("Register object failed; " << error);
        throwDbusException(error);
    }
}

void DbusConnection::onMethodCall(GDBusConnection*,
                                  const gchar*,
                                  const gchar* objectPath,
                                  const gchar* interface,
                                  const gchar* method,
                                  GVariant* parameters,
                                  GDBusMethodInvocation* invocation,
                                  gpointer userData)
{
    const MethodCallCallback& callback = *static_cast<const MethodCallCallback*>(userData);

    LOGD("MethodCall; " << objectPath << "; " << interface << "; " << method);

    MethodResultBuilderImpl resultBuilder(invocation);
    if (callback) {
        callback(objectPath, interface, method, parameters, resultBuilder);
    }

    if (resultBuilder.isUndefined()) {
        resultBuilder.setError("org.freedesktop.DBus.Error.UnknownMethod", "Not implemented");
    }
}

GVariantPtr DbusConnection::callMethod(const std::string& busName,
                                       const std::string& objectPath,
                                       const std::string& interface,
                                       const std::string& method,
                                       GVariant* parameters,
                                       const std::string& replyType)
{
    ScopedError error;
    GVariant* result = g_dbus_connection_call_sync(mConnection,
                                                   busName.c_str(),
                                                   objectPath.c_str(),
                                                   interface.c_str(),
                                                   method.c_str(),
                                                   parameters,
                                                   G_VARIANT_TYPE(replyType.c_str()),
                                                   G_DBUS_CALL_FLAGS_NONE,
                                                   CALL_METHOD_TIMEOUT_MS,
                                                   NULL,
                                                   &error);
    if (error) {
        error.strip();
        LOGE("Call method failed; " << error);
        throwDbusException(error);
    }
    return GVariantPtr(result, g_variant_unref);
}

} // namespace dbus
} // namespace security_containers