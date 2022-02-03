// Copyright (C) 2022 Alain Delplanque

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <algorithm>
#include <cstdlib>
#include <unistd.h>

#include "env.h"


std::string environnement_t::at(const std::string& name) const
{
    std::string result;
    const char* value = std::getenv(name.c_str());
    if (value != nullptr) {
        result = value;
    }
    return result;
}

size_t environnement_t::size() const
{
    size_t result = 0;
    while (environ[result]) {
        ++result;
    }
    return result;
}

bool environnement_t::count(const std::string& name) const
{
    const char* value = std::getenv(name.c_str());
    return value != nullptr;
}

std::vector<std::string> environnement_t::keys() const
{
    std::vector<std::string> result;
    for (size_t i = 0; environ[i]; i++) {
        char* end = environ[i];
        while (*end != '\0' && *end != '=') {
            end++;
        }
        result.push_back(std::string(environ[i], end));
    }
    std::sort(result.begin(), result.end());
    return result;
}
