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

#ifndef __parser_h__
#define __parser_h__

#include <filesystem>
#include <list>
#include <memory>
#include <regex>
#include <string>

#include <iostream>


class option_values_t
{
private:
    typedef std::map<std::string, std::string> config_values_t;
    typedef std::pair<std::shared_ptr<std::filesystem::path>, config_values_t> filename_values_t;
    typedef std::list<filename_values_t> values_t;

public:
    option_values_t() {};
    option_values_t(const std::string& name)
        : name(name)
    {};
    option_values_t(option_values_t&& other)
        : name(std::move(other.name))
        , values(std::move(other.values))
    {};

    void add_value(const std::shared_ptr<std::filesystem::path>& filename,
                   const std::string& config, const std::string& value);
    const std::string& value(const std::string& config) const;
    std::string format_values();

private:
    std::string name;
    values_t values;
};

class comments_t {
public:
    comments_t() {}
    comments_t(comments_t&& other) : lines(std::move(other.lines)) {}
    comments_t& operator=(comments_t&& other)
    {
        this->lines = std::move(other.lines);
        return *this;
    };
    void append(std::list<std::string>&& lines);
    std::string format();
    size_t size() { return lines.size(); }

private:
    std::list<std::string> lines;
};

class parser_t
{
public:
    typedef std::shared_ptr<parser_t> ptr;
    // typedef std::pair<std::string, std::string> option_key_t;
    typedef std::map<
        std::string,
        std::pair<
            comments_t,
            std::map<
                std::string,
                std::pair<comments_t,
                          option_values_t
                          >
                >
            >
        > options_t;

    parser_t() {};
    void load_file(std::shared_ptr<std::filesystem::path> filename);
    bool empty() { return options.empty(); }
    options_t& get_options() { return options; };

private:

    static const std::regex group_regex;
    static const std::regex comment_regex;
    static const std::regex option_regex;
    static const std::regex empty_regex;

    options_t options;
};

#endif
