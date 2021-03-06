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
 * @brief   Declaration of the class for storing zone manager configuration
 */


#ifndef SERVER_ZONES_MANAGER_CONFIG_HPP
#define SERVER_ZONES_MANAGER_CONFIG_HPP

#include "cargo/fields.hpp"
#include "input-monitor-config.hpp"
#include "proxy-call-config.hpp"

#include <string>
#include <vector>


namespace vasum {

struct ZonesManagerConfig {

    /**
     * Path to config database.
     */
    std::string dbPath;

    /**
     * A path where the zones mount points reside.
     */
    std::string zonesPath;

    /**
     * If set then all files not related with existing containers will be removed
     */
    bool cleanUpZonesPath;

    /**
     * A path where the zones image reside. Empty path means that zones image won't be
     * copied to zonesPath when creating new zone.
     */
    std::string zoneImagePath;

    /**
     * A dir where template configuration files for new zones reside
     */
    std::string zoneTemplateDir;

    /**
     * Host VT
     */
    int hostVT;

    /**
     * VTs available for zones
     */
    std::vector<int> availableVTs;

    /*
     * Parameters describing input device used to switch between zones
     */
    InputConfig inputConfig;

    /**
     * Prefix added to a path of "run" tmpfs mount point for each zone.
     */
    std::string runMountPointPrefix;

    /**
     * Proxy call rules.
     */
    std::vector<ProxyCallRule> proxyCallRules;

    CARGO_REGISTER
    (
        dbPath,
        zonesPath,
        cleanUpZonesPath,
        zoneImagePath,
        zoneTemplateDir,
        hostVT,
        availableVTs,
        inputConfig,
        runMountPointPrefix,
        proxyCallRules
    )
};

struct ZonesManagerDynamicConfig {

    /**
     * A list of created zones.
     */
    std::vector<std::string> zoneIds;

    /**
     * An ID of default zone.
     */
    std::string defaultId;

    CARGO_REGISTER
    (
        zoneIds,
        defaultId
    )
};

} // namespace vasum


#endif // SERVER_ZONES_MANAGER_CONFIG_HPP
