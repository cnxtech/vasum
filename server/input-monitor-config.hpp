/*
 *  Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Pawel Broda <p.broda@partner.samsung.com>
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
 * @author  Pawel Broda (p.broda@partner.samsung.com)
 * @brief   Declaration of the struct for storing input monitor configuration
 */


#ifndef SERVER_INPUT_MONITOR_CONFIG_HPP
#define SERVER_INPUT_MONITOR_CONFIG_HPP


#include "config/configuration.hpp"

#include <string>


namespace security_containers {

struct InputConfig : public config::ConfigurationBase {

    bool enabled;

    /**
     * represents either a device name or an absolute device file path (must be a string starting
     *  with '/' character)
     */
    std::string device;
    int code;
    int numberOfEvents;
    int timeWindowMs;

    CONFIG_REGISTER {
        CONFIG_VALUE(enabled)
        CONFIG_VALUE(device)
        CONFIG_VALUE(code)
        CONFIG_VALUE(numberOfEvents)
        CONFIG_VALUE(timeWindowMs)
    }

};

} // namespace security_containers

#endif /* SERVER_INPUT_MONITOR_CONFIG_HPP */
