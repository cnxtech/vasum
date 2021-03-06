/*
*  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
*
*  Contact: Jan Olszak (j.olszak@samsung.com)
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
 * @file
 * @author  Jan Olszak (j.olszak@samsung.com)
 * @brief   Interface for result builders

 */

#ifndef COMMON_RESULT_DBUS_METHOD_RESULT_BUILDER_HPP
#define COMMON_RESULT_DBUS_METHOD_RESULT_BUILDER_HPP

#include "api/method-result-builder.hpp"

#include "dbus/connection.hpp"
#include "cargo-gvariant/cargo-gvariant.hpp"

#include <memory>
#include <functional>

namespace vasum {
namespace api {

const std::string DBUS_CONNECTION_PREFIX = "dbus://";

template<typename Data>
class DbusMethodResultBuilder: public MethodResultBuilder {
public:
    DbusMethodResultBuilder(const ::dbus::MethodResultBuilder::Pointer& methodResultBuilderPtr);
    ~DbusMethodResultBuilder() {}

private:
    void setImpl(const std::shared_ptr<void>& data) override;
    void setVoid() override;
    void setError(const std::string& name, const std::string& message) override;
    std::string getID() const override;

    ::dbus::MethodResultBuilder::Pointer mMethodResultBuilderPtr;
    std::function<GVariant*(std::shared_ptr<void>)> mSerialize;
};

template<typename Data>
DbusMethodResultBuilder<Data>::DbusMethodResultBuilder(const ::dbus::MethodResultBuilder::Pointer& methodResultBuilderPtr)
    : mMethodResultBuilderPtr(methodResultBuilderPtr)
{
    mSerialize = [](const std::shared_ptr<void> data)->GVariant* {
        return cargo::saveToGVariant(*std::static_pointer_cast<Data>(data));
    };
}

template<typename Data>
void DbusMethodResultBuilder<Data>::setImpl(const std::shared_ptr<void>& data)
{
    GVariant* parameters = mSerialize(data);
    mMethodResultBuilderPtr->set(parameters);
}

template<typename Data>
void DbusMethodResultBuilder<Data>::setVoid()
{
    mMethodResultBuilderPtr->setVoid();
}

template<typename Data>
void DbusMethodResultBuilder<Data>::setError(const std::string& name, const std::string& message)
{
    mMethodResultBuilderPtr->setError(name, message);
}

template<typename Data>
std::string DbusMethodResultBuilder<Data>::getID() const
{
    return DBUS_CONNECTION_PREFIX + mMethodResultBuilderPtr->getPeerName();
}

} // namespace result
} // namespace vasum

#endif // COMMON_RESULT_DBUS_METHOD_RESULT_BUILDER_HPP
