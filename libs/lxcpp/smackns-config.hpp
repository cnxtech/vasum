/*
 *  Copyright (C) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * @file
 * @author  Lukasz Kostyra (l.kostyra@samsumg.com)
 * @brief   Smack namespace configuration
 */

#ifndef LXCPP_SMACKNS_CONFIG_HPP
#define LXCPP_SMACKNS_CONFIG_HPP

#include "cargo/fields.hpp"

#include <vector>
#include <string>


namespace lxcpp {


struct SmackNSConfig
{
    // original label, mapped label
    std::vector<std::tuple<std::string, std::string>> mLabelMap;

    CARGO_REGISTER
    (
        mLabelMap
    )
};


} // namespace lxcpp


#endif // LXCPP_SMACKNS_CONFIG_HPP
