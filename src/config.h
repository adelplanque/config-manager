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

#ifndef __config_h__
#define __config_h__

#include <list>
#include <filesystem>

#include <iostream>

class config_t {

public:
    static config_t& get_instance()
    {
        if (config_t::instance == nullptr) {
            config_t::instance = new config_t();
        }
        return *config_t::instance;
    }

    const std::list<std::filesystem::path>& get_config_path() const { return config_path; }
    std::string& get_config_name() { return config_name; }
    void append_config_path(const std::string& path);

private:
    config_t();
    static config_t* instance;

    std::list<std::filesystem::path> config_path;
    std::string config_name;
};

#endif
