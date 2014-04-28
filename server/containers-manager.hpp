/*
 *  Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Jan Olszak <j.olszak@samsung.com>
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
 * @brief   Declaration of the class for managing many containers
 */


#ifndef SERVER_CONTAINERS_MANAGER_HPP
#define SERVER_CONTAINERS_MANAGER_HPP

#include "container.hpp"
#include "containers-manager-config.hpp"

#include <string>
#include <unordered_map>
#include <libvirt/libvirt.h>


namespace security_containers {


class ContainersManager final {

public:
    ContainersManager(const std::string& managerConfigPath);
    ~ContainersManager();

    /**
     * Focus this container, put it to the foreground.
     * Method blocks until the focus is switched.
     *
     * @param containerId id of the container
     */
    void focus(const std::string& containerId);

    /**
     * Start up all the configured containers
     */
    void startAll();

    /**
     * Stop all managed containers
     */
    void stopAll();

    /**
     * @return id of the currently focused/foreground container
     */
    std::string getRunningForegroundContainerId();

private:
    ContainersManagerConfig mConfig;
    typedef std::unordered_map<std::string, std::unique_ptr<Container>> ContainerMap;
    ContainerMap mContainers; // map of containers, id is the key
};


}


#endif // SERVER_CONTAINERS_MANAGER_HPP