/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @brief   glib epoll dispatcher
 */

#ifndef COMMON_EPOLL_GLIB_DISPATCHER_HPP
#define COMMON_EPOLL_GLIB_DISPATCHER_HPP

#include "epoll/event-poll.hpp"
#include "utils/callback-guard.hpp"

#include <gio/gio.h>

namespace vasum {
namespace epoll {

/**
 * Will dispatch poll events in glib thread
 */
class GlibDispatcher {
public:
    GlibDispatcher();
    ~GlibDispatcher();

    EventPoll& getPoll();
private:
    EventPoll mPoll; // before mGuard!
    utils::CallbackGuard mGuard;
    GIOChannel* mChannel;
    guint mWatchId;
};


} // namespace epoll
} // namespace vasum

#endif // COMMON_UTILS_GLIB_DISPATCHER_HPP