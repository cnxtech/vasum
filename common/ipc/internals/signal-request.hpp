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
 * @brief   Processor's request to send a signal
 */

#ifndef COMMON_IPC_INTERNALS_SIGNAL_REQUEST_HPP
#define COMMON_IPC_INTERNALS_SIGNAL_REQUEST_HPP

#include "ipc/types.hpp"
#include "config/manager.hpp"
#include "logger/logger-scope.hpp"

namespace vasum {
namespace ipc {

class SignalRequest {
public:
    SignalRequest(const SignalRequest&) = delete;
    SignalRequest& operator=(const SignalRequest&) = delete;



    template<typename SentDataType>
    static std::shared_ptr<SignalRequest> create(const MethodID methodID,
                                                 const FileDescriptor peerFD,
                                                 const std::shared_ptr<SentDataType>& data);

    MethodID methodID;
    FileDescriptor peerFD;
    MessageID messageID;
    std::shared_ptr<void> data;
    SerializeCallback serialize;

private:
    SignalRequest(const MethodID methodID, const FileDescriptor peerFD)
        : methodID(methodID),
          peerFD(peerFD),
          messageID(getNextMessageID())
    {}

};

template<typename SentDataType>
std::shared_ptr<SignalRequest> SignalRequest::create(const MethodID methodID,
                                                     const FileDescriptor peerFD,
                                                     const std::shared_ptr<SentDataType>& data)
{
    std::shared_ptr<SignalRequest> request(new SignalRequest(methodID, peerFD));

    request->data = data;

    request->serialize = [](const int fd, std::shared_ptr<void>& data)->void {
        LOGS("Signal serialize, peerFD: " << fd);
        config::saveToFD<SentDataType>(fd, *std::static_pointer_cast<SentDataType>(data));
    };

    return request;
}

} // namespace ipc
} // namespace vasum

#endif // COMMON_IPC_INTERNALS_SIGNAL_REQUEST_HPP