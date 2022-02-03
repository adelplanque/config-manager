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

#ifndef __settings_h__
#define __settings_h__

#include <map>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "fmt/format.h"

#include <iostream>

#include "parser.h"


class value_t
{
public:
    value_t(const std::string& value)
        : value(value)
    {}
    value_t(const std::string& value, option_values_t&& config_values)
        : value(value)
        , config_values(std::move(config_values))
    {}
    value_t(value_t&& other)
        : value(std::move(other.value))
        , config_values(std::move(other.config_values))
    {}

    template<typename T> T as() { return boost::lexical_cast<T>(value); }
    std::string& get() { return this->value; }
    std::string format_values() { return config_values.format_values(); }

private:
    std::string value;
    option_values_t config_values;
};


class settings_t : public std::enable_shared_from_this<settings_t>
{
public:
    typedef std::shared_ptr<settings_t> settings_ptr;
    typedef std::map<std::string, settings_ptr> mapping_t;
    typedef std::optional<mapping_t::iterator> iterator;
    typedef std::variant<mapping_t, value_t> content_t;
    enum type_t {mapping, value};

public:
    settings_t()
        : parent_(nullptr)
        , content_(mapping_t())
        , is_loaded(false)
    {};
    settings_t(const std::string name, settings_ptr parent=nullptr)
        : name_(name)
        , parent_(parent)
        , content_(mapping_t())
        , is_loaded(false)
    {};
    settings_t(const std::string name, value_t&& value, settings_ptr parent=nullptr)
        : name_(name)
        , parent_(parent)
        , content_(std::move(value))
        , is_loaded(true)
    {};

    settings_ptr at(const std::string& key);
    settings_ptr get_or_create(const std::string& key);
    void append(const std::string& key, settings_ptr& settings);
    size_t count(const std::string& key);
    std::vector<std::string> keys();
    void doc(std::ostream& os);

    template<typename T> T as()
    {
        if (std::holds_alternative<value_t>(content_)) {
            return std::get<value_t>(content_).as<T>();
        }
        throw std::out_of_range(fmt::format("Key error: {}", full_name()));
    }

    type_t type()
    {
        if (std::holds_alternative<mapping_t>(content_)) {
            return mapping;
        } else {
            return value;
        }
    }

    template<typename T> bool is() { return std::holds_alternative<T>(this->content_); }

    size_t size()
    {
        std::cerr << __PRETTY_FUNCTION__ << std::endl;
        load();
        if (this->type() == mapping) {
            std::cerr << __PRETTY_FUNCTION__ << ": " << std::get<mapping_t>(content_).size() << std::endl;
            return std::get<mapping_t>(content_).size();
        } else {
            return 0;
        }
    }

    std::string full_name();
    std::out_of_range out_of_range(const std::string& key);
    std::filesystem::path get_path();
    settings_ptr get_root()
    {
        return (this->parent_ == nullptr) ? shared_from_this() : this->parent_->get_root();
    }
    void set_comments(comments_t&& comments) { this->comments = std::move(comments); }

    iterator begin()
    {
        if (! this->is_loaded) {
            this->load();
        }
        if (std::holds_alternative<mapping_t>(this->content_)) {
            return std::get<mapping_t>(this->content_).begin();
        } else {
            return std::nullopt;
        }
    }

    iterator end()
    {
        if (! this->is_loaded) {
            this->load();
        }
        if (std::holds_alternative<mapping_t>(this->content_)) {
            return std::get<mapping_t>(this->content_).end();
        } else {
            return std::nullopt;
        }
    }

private:
    std::string name_;
    settings_ptr parent_;
    content_t content_;
    bool is_loaded;
    comments_t comments;

    void load();
    void load_file();
};

#endif
