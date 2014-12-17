/*
 *  Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Piotr Bartosiewicz <p.bartosiewi@partner.samsung.com>
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
 * @author  Piotr Bartosiewicz (p.bartosiewi@partner.samsung.com)
 * @brief   Zone dbus api definitions
 */

#ifndef SERVER_ZONE_DBUS_DEFINITIONS_HPP
#define SERVER_ZONE_DBUS_DEFINITIONS_HPP

#include "common-dbus-definitions.hpp"


namespace vasum {
namespace api {
namespace zone {

const std::string BUS_NAME                          = "org.tizen.vasum.zone";
const std::string OBJECT_PATH                       = "/org/tizen/vasum/zone";
const std::string INTERFACE                         = "org.tizen.vasum.zone.manager";

const std::string METHOD_NOTIFY_ACTIVE_ZONE    = "NotifyActiveZone";
const std::string METHOD_FILE_MOVE_REQUEST          = "FileMoveRequest";
const std::string SIGNAL_NOTIFICATION               = "Notification";

const std::string FILE_MOVE_DESTINATION_NOT_FOUND   = "FILE_MOVE_DESTINATION_NOT_FOUND";
const std::string FILE_MOVE_WRONG_DESTINATION       = "FILE_MOVE_WRONG_DESTINATION";
const std::string FILE_MOVE_NO_PERMISSIONS_SEND     = "FILE_MOVE_NO_PERMISSIONS_SEND";
const std::string FILE_MOVE_NO_PERMISSIONS_RECEIVE  = "FILE_MOVE_NO_PERMISSIONS_RECEIVE";
const std::string FILE_MOVE_FAILED                  = "FILE_MOVE_FAILED";
const std::string FILE_MOVE_SUCCEEDED               = "FILE_MOVE_SUCCEEDED";


const std::string DEFINITION =
    "<node>"
    "  <interface name='" + INTERFACE + "'>"
    "    <method name='" + METHOD_NOTIFY_ACTIVE_ZONE + "'>"
    "      <arg type='s' name='application' direction='in'/>"
    "      <arg type='s' name='message' direction='in'/>"
    "    </method>"
    "    <method name='" + METHOD_FILE_MOVE_REQUEST + "'>"
    "      <arg type='s' name='destination' direction='in'/>"
    "      <arg type='s' name='path' direction='in'/>"
    "      <arg type='s' name='result' direction='out'/>"
    "    </method>"
    "    <method name='" + METHOD_PROXY_CALL + "'>"
    "      <arg type='s' name='target' direction='in'/>"
    "      <arg type='s' name='busName' direction='in'/>"
    "      <arg type='s' name='objectPath' direction='in'/>"
    "      <arg type='s' name='interface' direction='in'/>"
    "      <arg type='s' name='method' direction='in'/>"
    "      <arg type='v' name='parameters' direction='in'/>"
    "      <arg type='v' name='result' direction='out'/>"
    "    </method>"
    "    <signal name='" + SIGNAL_NOTIFICATION + "'>"
    "      <arg type='s' name='zone'/>"
    "      <arg type='s' name='application'/>"
    "      <arg type='s' name='message'/>"
    "    </signal>"
    "  </interface>"
    "</node>";

} // namespace zone
} // namespace api
} // namespace vasum


#endif // SERVER_ZONE_DBUS_DEFINITIONS_HPP