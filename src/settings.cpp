#include <filesystem>
#include <list>
#include <optional>
#include <stdexcept>
#include <string>

#include "parser.h"
#include "settings.h"


std::list<std::filesystem::path> config_search_paths = {
    std::filesystem::path("/home/alain/synopsis/config-manager/tests/config-local"),
    std::filesystem::path("/home/alain/synopsis/config-manager/tests/config-global")
};

std::string config_name = "OPER";


settings_t::settings_ptr settings_t::at(const std::string& key)
{
    std::cout << "lock for key: " << key << std::endl;
    try {
        mapping_t& mapping = std::get<mapping_t>(content_);
        if (mapping.count(key)) {
            return mapping.at(key);
        }
    }
    catch (const std::bad_variant_access& ex) {
        throw out_of_range(key);
    }

    std::filesystem::path p = get_path();
    std::cout << "path: " << p << std::endl;
    for (const auto& config_path : config_search_paths) {
        if (std::filesystem::is_directory(config_path / p / key)) {
            std::cout << "Open new direcory: " << (config_path / p / key) << std::endl;
            mapping_t& mapping = std::get<mapping_t>(content_);
            settings_ptr dir_settings { new settings_t(key, shared_from_this()) };
            mapping[key] = dir_settings;
            return dir_settings;
        }
    }

    return load_file(key);
}

std::string settings_t::full_name()
{
    std::string name = (parent_ == nullptr) ? "" : parent_->full_name();
    if (! name_.empty()) {
        name.append(".");
        name.append(name_);
    }
    return name;
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

settings_t::settings_ptr settings_t::load_file(const std::string& key)
{
    parser_t parser;
    std::filesystem::path p = get_path();
    for (const auto config_path : config_search_paths) {
        auto filename = config_path / p / (key + ".ini");
        std::cout << "lock for file: " << filename << std::endl;
        if (std::filesystem::is_regular_file(filename)) {
            parser.load_file(filename);
        }
    }

    if (parser.empty()) {
        throw out_of_range(key);
    }

    if (parser.get_options().empty()) {
        throw std::out_of_range("");
    }

    settings_ptr file_settings = get_or_create(key);
    for (auto const& [option_key, option_value] : parser.get_options()) {
        file_settings->get_or_create(option_key.first)
            ->set_value(option_key.second, option_value->value(config_name));
    }

    return file_settings;
}

settings_t::settings_ptr settings_t::get_or_create(const std::string& key) {
    try {
        mapping_t& mapping = std::get<mapping_t>(content_);
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

void settings_t::set_value(const std::string& key, const value_t& value) {
    try {
        mapping_t& mapping = std::get<mapping_t>(content_);
        mapping.emplace(key, new settings_t(key, value, shared_from_this()));
    }
    catch (std::bad_variant_access&) {
        throw out_of_range(key);
    }
}
