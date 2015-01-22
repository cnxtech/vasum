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
 * @brief   Processor's request to add a peer
 */

#ifndef COMMON_IPC_INTERNALS_ADD_PEER_REQUEST_HPP
#define COMMON_IPC_INTERNALS_ADD_PEER_REQUEST_HPP

#include "ipc/types.hpp"
#include "ipc/internals/socket.hpp"

namespace vasum {
namespace ipc {

class AddPeerRequest {
public:
    AddPeerRequest(const AddPeerRequest&) = delete;
    AddPeerRequest& operator=(const AddPeerRequest&) = delete;

    AddPeerRequest(const FileDescriptor peerFD, const std::shared_ptr<Socket>& socketPtr)
        : peerFD(peerFD),
          socketPtr(socketPtr)
    {
    }

    FileDescriptor peerFD;
    std::shared_ptr<Socket> socketPtr;
};

} // namespace ipc
} // namespace vasum

#endif // COMMON_IPC_INTERNALS_ADD_PEER_REQUEST_HPP