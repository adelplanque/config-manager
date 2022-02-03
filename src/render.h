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

#ifndef __render_h__
#define __render_h__

#include <istream>
#include <list>
#include <sstream>
#include <string>

#include <jinja2cpp/template.h>

#include "settings.h"


class Renderer
{
public:
    Renderer(settings_t::settings_ptr settings);
    std::string render(std::istream& is);
    std::string render(std::string& str)
    {
        std::cerr << __PRETTY_FUNCTION__ << ": " << str << std::endl;
        auto is = std::istringstream(str);
        auto res = this->render(is);
        std::cerr << __PRETTY_FUNCTION__ << ": " << str << " => " << res << std::endl;
        return res;
    }
    void push_back(const std::string& key)
    {
        std::cerr << __PRETTY_FUNCTION__ << ": " << key << std::endl;
        if (std::find(this->stack.begin(), this->stack.end(), key) != this->stack.end()) {
            std::cerr << __PRETTY_FUNCTION__ << " recursive" << std::endl;
            throw std::runtime_error("recursive loop");
        }
        this->stack.push_back(key);
    }
    void pop() { std::cerr << "pop" << std::endl; this->stack.pop_back(); }

private:
    jinja2::ValuesMap values;
    std::list<std::string> stack;
};

std::string render(settings_t::settings_ptr settings, std::istream& is);


#endif
