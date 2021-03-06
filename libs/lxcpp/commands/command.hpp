/*
 *  Copyright (C) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @author  Jan Olszak (j.olszak@samsung.com)
 * @brief   Command interface definition
 */

#ifndef LXCPP_COMMANDS_COMMAND_HPP
#define LXCPP_COMMANDS_COMMAND_HPP

namespace lxcpp {

class Command {
public:
    // do sth [mandatory]
    virtual void execute() = 0;

    // roll-back execute() action [optional]
    virtual void revert() {}
};

} // namespace lxcpp

#endif // LXCPP_COMMANDS_COMMAND_HPP
