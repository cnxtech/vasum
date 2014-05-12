/*
 *  Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @brief   Server class declaration
 */


#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include "utils/latch.hpp"

#include <string>


namespace security_containers {


class Server {
public:
    Server(const std::string& configPath, bool runAsRoot = true);
    virtual ~Server();

    /**
     * Set needed caps, groups and drop root privileges.
     */
    static bool prepareEnvironment(const std::string& configPath, bool runAsRoot);

    /**
     * Starts all the containers and blocks until SIGINT, SIGTERM or SIGUSR1
     */
    void run();

    /**
     * Reload the server by launching execve on itself if SIGUSR1 was sent to server.
     */
    void reloadIfRequired(char* argv[]);

    /**
     * Terminates the server.
     * Equivalent of sending SIGINT or SIGTERM signal
     */
    void terminate();
private:
    std::string mConfigPath;
};


} // namespace security_containers


#endif // SERVER_SERVER_HPP
