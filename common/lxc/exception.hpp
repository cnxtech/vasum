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
 * @brief   Lxc exception
 */


#ifndef COMMON_LXC_EXCEPTION_HPP
#define COMMON_LXC_EXCEPTION_HPP

#include "base-exception.hpp"


namespace vasum {


/**
 * Base class for exceptions in utils
 */
struct LxcException: public VasumException {

    explicit LxcException(const std::string& error) : VasumException(error) {}
};

struct KeyNotFoundException: public LxcException {

    explicit KeyNotFoundException(const std::string& error) : LxcException(error) {}
};

} // namespace vasum

#endif // COMMON_UTILS_EXCEPTION_HPP
