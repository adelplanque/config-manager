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

#include <filesystem>
#include <list>
#include <optional>
#include <stdexcept>
#include <string>
#include <set>

#include <boost/range/adaptor/reversed.hpp>

#include "config.h"
#include "parser.h"
#include "render.h"
#include "settings.h"


settings_t::settings_ptr settings_t::at(const std::string& key)
{
    if (! this->is_loaded) {
        this->load();
    }
    try {
        return std::get<mapping_t>(content_).at(key);
    }
    catch (const std::bad_variant_access&) {
        throw out_of_range(key);
    }
    catch (const std::out_of_range&) {
        throw out_of_range(key);
    }
}

std::string settings_t::full_name()
{
    std::string result = (parent_ == nullptr) ? "" : parent_->full_name();
    if (! result.empty()) {
        result.append(".");
    }
    result.append(name_);
    return result;
}

std::out_of_range settings_t::out_of_range(const std::string& key) {
    return std::out_of_range(fmt::format("Key {} undefined in .{}", key, full_name()));
}

std::filesystem::path settings_t::get_path()
{
    std::filesystem::path p =
        (parent_ == nullptr) ? std::filesystem::path() : parent_->get_path();
    if (! name_.empty()) {
        p /= name_;
    }
    return p;
}

void settings_t::load_file()
{
    std::cerr << __PRETTY_FUNCTION__ << ": node=" << this->full_name() << std::endl;
    if (this->parent_ == nullptr) {
        return;
    }
    parser_t parser;
    std::list<std::shared_ptr<std::filesystem::path>> filenames;
    std::filesystem::path p = get_path();
    for (const auto& config_path
             : boost::adaptors::reverse(config_t::get_instance().get_config_path())) {
        auto node_path = config_path / this->get_path();
        auto filename = node_path.parent_path() / (node_path.filename().native() + ".ini");
        if (std::filesystem::is_regular_file(filename)) {
            parser.load_file(std::make_shared<std::filesystem::path>(std::move(filename)));
        }
    }

    const auto& config_name = config_t::get_instance().get_config_name();
    for (auto& [group_name, group] : parser.get_options()) {
        auto& [group_comments, group_map] = group;
        auto group_settings = this->get_or_create(group_name);
        group_settings->set_comments(std::move(group_comments));
        for (auto& [option_name, option] : group_map) {
            auto& [option_comments, option_values] = option;
            auto option_settings
                = std::make_shared<settings_t>
                (option_name,
                 value_t(option_values.value(config_name), std::move(option_values)),
                 group_settings
                 );
            group_settings->append(option_name, option_settings);
            option_settings->set_comments(std::move(option_comments));
        }
    }

    Renderer r { this->get_root() };
    for (auto& group : std::get<mapping_t>(this->content_)) {
        std::cerr << "load_file iter group " << group.first << std::endl;
        if (! group.second->is<mapping_t>()) {
            continue;
        }
        for (auto& option : std::get<mapping_t>(group.second->content_)) {
            if (! option.second->is<value_t>()) {
                continue;
            }
            std::cerr << "load_file iter option " << option.first << std::endl;
            auto& value = std::get<value_t>(option.second->content_).get();
            if (value.find("{{") != std::string::npos || value.find("{%") != std::string::npos) {
                std::string initial = value;
                std::cerr << "load_file: render " << initial << std::endl;
                value = r.render(value);
                std::cerr << "load_file: render " << initial << " => " << value << std::endl;
            }
        }
    }
}

settings_t::settings_ptr settings_t::get_or_create(const std::string& key) {
    try {
        mapping_t& mapping = std::get<mapping_t>(this->content_);
        try {
            return mapping.at(key);
        }
        catch (const std::out_of_range&) {
            settings_ptr settings = std::make_shared<settings_t>(key, shared_from_this());
            mapping[key] = settings;
            return settings;
        }
    }
    catch (std::bad_variant_access&) {
        throw out_of_range(key);
    }
}

void settings_t::append(const std::string& key, settings_t::settings_ptr& settings) {
    try {
        mapping_t& mapping = std::get<mapping_t>(content_);
        mapping.emplace(key, settings);
    }
    catch (std::bad_variant_access&) {
        throw out_of_range(key);
    }
}

void settings_t::load()
{
    if (is_loaded) {
        return;
    }
    std::cerr << __PRETTY_FUNCTION__ << ": " << full_name() << ": start" << std::endl;
    is_loaded = true;

    // Lookup if this node is a file
    this->load_file();

    // Scan directory for files or subdirectories for keys
    std::set<std::string> keys;
    std::filesystem::path p = get_path();
    for (const auto& config_path
             : boost::adaptors::reverse(config_t::get_instance().get_config_path())) {
        if (! std::filesystem::is_directory(config_path / p)) {
            continue;
        }
        for (const auto& entry : std::filesystem::directory_iterator(config_path / p)) {
            if (entry.is_directory()) {
                const auto& dir_name = entry.path().filename().native();
                if (dir_name.find(".") == std::string::npos) {
                    keys.insert(std::move(dir_name));
                }
            } else if (entry.is_regular_file() && entry.path().extension() == ".ini") {
                const auto& filename = entry.path().stem().native();
                if (filename.find(".") == std::string::npos) {
                    keys.insert(std::move(filename));
                }
            }
        }
    }

    // Create node for keys
    for (const auto& key : keys) {
        this->get_or_create(key);
    }
    std::cerr << __PRETTY_FUNCTION__ << " " << this->full_name() << ": end" << std::endl;
}

size_t settings_t::count(const std::string& key)
{
    load();
    try {
        return std::get<mapping_t>(content_).count(key);
    }
    catch (const std::bad_variant_access&) {
        return 0;
    }
}

std::vector<std::string> settings_t::keys()
{
    load();
    if (this->type() != mapping) {
        throw std::out_of_range(fmt::format("Key error: {} is a leaf object", full_name()));
    }
    auto mapping = std::get<mapping_t>(content_);
    std::vector<std::string> result;
    for (const auto& item : mapping) {
        result.push_back(item.first);
    }
    return result;
}

void settings_t::doc(std::ostream& os)
{
    if (this->type() == value) {
        std::string header = fmt::format("Option {}", this->full_name());
        os << header << std::endl
           << std::string(header.size(), '.') << std::endl << std::endl;
        if (this->comments.size() > 0) {
            os << this->comments.format() << std::endl;
        }
        os << std::get<value_t>(content_).format_values();
    } else {
        std::string header = fmt::format("Group {}", this->full_name());
        os << header << std::endl
           << std::string(header.size(), '-') << std::endl;
        if (this->comments.size() > 0) {
            os << std::endl << this->comments.format();
        }
        auto keys = this->keys();
        if (! keys.empty()) {
            os << std::endl;
            std::sort(keys.begin(), keys.end());
            for (const auto& key : keys) {
                this->at(key)->doc(os);
                if (&key != &keys.back()) {
                    os << std::endl;
                }
            }
        }
    }
}
