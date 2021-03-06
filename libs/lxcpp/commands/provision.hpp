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
 * @author  Maciej Karpiuk (m.karpiuk2@samsung.com)
 * @brief   Add new provisioned file/dir/link/mount command
 */

#ifndef LXCPP_COMMANDS_PROVISION_HPP
#define LXCPP_COMMANDS_PROVISION_HPP

#include "lxcpp/commands/command.hpp"
#include "lxcpp/container-config.hpp"
#include "lxcpp/provision-config.hpp"

#include <sys/types.h>

namespace lxcpp {

class Provisions final: Command {
public:
   /**
    * Runs call in the container's context
    *
    * Add/remove all file/fifo/dir/mount/link provisions to/from the container
    */
    Provisions(ContainerConfig &config) : mConfig(config)
    {
    }

    void execute();
    void revert();

private:
    ContainerConfig& mConfig;
};


class ProvisionFile final: Command {
public:
   /**
    * Runs call in the container's context
    *
    * Add/remove new file/fifo/dir provision to/from the container
    */
    ProvisionFile(const provision::File &file);

    void execute();
    void revert();

private:
    const provision::File& mFile;
};

class ProvisionMount final: Command {
public:
   /**
    * Runs call in the container's context
    *
    * Add/remove new mount provision to/from the container
    */
    ProvisionMount(const provision::Mount &mount);

    void execute();
    void revert();

private:
    const provision::Mount& mMount;
};

class ProvisionLink final: Command {
public:
   /**
    * Runs call in the container's context
    *
    * Add/remove link provision to/from the container
    */
    ProvisionLink(const provision::Link &link);

    void execute();
    void revert();

private:
    const provision::Link& mLink;
};

} // namespace lxcpp

#endif // LXCPP_COMMANDS_PROVISION_HPP
