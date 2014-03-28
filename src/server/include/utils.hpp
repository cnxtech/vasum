/*
 *  Copyright (c) 2000 - 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Bumjin Im <bj.im@samsung.com>
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
 * @file    utils.hpp
 * @author  Jan Olszak (j.olszak@samsung.com)
 * @brief   Utility function declaration
 */


#include <string>
#include <vector>
#include <algorithm>


namespace security_containers {
namespace utils {

template <class ...Paths> std::string createFilePath(const Paths& ... paths)
{
    std::vector<std::string> pathVec = {paths...};
    std::string retPath = "";

    if (pathVec.empty()) {
        return retPath;
    }

    for (std::string& p : pathVec) {
        // Repeat until retPath is not empty
        if (retPath.empty() || p.empty()) {
            retPath += p;
            continue;
        }

        // We need a slash
        if (retPath.back() != '/' && p.front() != '/' && p.front() != '.') {
            retPath += "/" + p;
            continue;
        }

        // Too many slashes
        if (retPath.back() == '/' && p.front() == '/') {
            retPath += p.substr(1);
            continue;
        }

        retPath += p;
    }

    return retPath;
}

namespace {

inline void removeDuplicateSlashes(std::string& path)
{
    auto it = std::unique(path.begin(), path.end(),
                          [](char a, char b) {
                              return (a == '/' && a == b);
                          });
    path.erase(it, path.end());
}

inline void removeTrailingSlash(std::string& path)
{
    if (path.size() > 1 && *(path.end() - 1) == '/') {
        path.pop_back();
    }
}

} // anonymous namespace

/*
 * Gets the dir name of a file path, analogous to dirname(1)
 */
inline std::string dirName(std::string path)
{
    removeDuplicateSlashes(path);
    removeTrailingSlash(path);
    path.erase(std::find(path.rbegin(), path.rend(), '/').base(), path.end());
    removeTrailingSlash(path);

    if (path.empty()) {
        return ".";
    }

    return path;
}

} // namespace utils
} // namespace security_containers