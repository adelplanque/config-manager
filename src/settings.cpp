#include <filesystem>
#include <list>
#include <optional>
#include <stdexcept>
#include <string>
#include <set>

#include "parser.h"
#include "settings.h"
#include "config.h"


settings_t::settings_ptr settings_t::at(const std::string& key)
{
    std::cerr << "lock for key: " << key << std::endl;
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
    std::cerr << "path: " << p << std::endl;
    for (const auto& config_path : config_t::get_instance().get_config_path()) {
        if (std::filesystem::is_directory(config_path / p / key)) {
            std::cerr << "Open new direcory: " << (config_path / p / key) << std::endl;
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
    for (const auto& config_path : config_t::get_instance().get_config_path()) {
        auto filename = config_path / p / (key + ".ini");
        std::cerr << "lock for file: " << filename << std::endl;
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
            ->set_value(option_key.second,
                        option_value->value(config_t::get_instance().get_config_name()));
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

void settings_t::load()
{
    std::cerr << __PRETTY_FUNCTION__
              << ": start (." << full_name() << " at " << this << ")" << std::endl;
    if (is_loaded) {
        std::cerr << "nothing to do" << std::endl;
        return;
    }
    std::set<std::string> filenames;
    std::filesystem::path p = get_path();
    for (const auto& config_path : config_t::get_instance().get_config_path()) {
        std::cerr << "config_path: " << (config_path / p) << std::endl;
        if (! std::filesystem::is_directory(config_path / p)) {
            continue;
        }
        for (const auto& entry : std::filesystem::directory_iterator(config_path / p)) {
            std::cerr << "entry: " << entry << std::endl;
            if (entry.is_directory()) {
                const auto& dir_name = entry.path().filename().native();
                if (dir_name.find(".") == std::string::npos) {
                    try {
                        mapping_t& mapping = std::get<mapping_t>(content_);
                        if (mapping.count(dir_name) == 0) {
                                mapping.emplace(dir_name,
                                                new settings_t(dir_name, shared_from_this()));
                            }
                    }
                    catch (const std::bad_variant_access&) {}
                }
            } else if (entry.is_regular_file() && entry.path().extension() == ".ini") {
                const auto& filename = entry.path().stem().native();
                std::cerr << "regular file OK, key=" << filename
                          << " . at " << filename.find(".") << std::endl;
                if (filename.find(".") == std::string::npos) {
                    std::cerr << "filename OK" << std::endl;
                    try {
                        mapping_t& mapping = std::get<mapping_t>(content_);
                        if (mapping.count(filename) == 0) {
                            std::cerr << "Insert " << filename << std::endl;
                            filenames.insert(filename);
                        }
                    }
                    catch (const std::bad_variant_access&) {}
                }
            }
        }
    }
    for (const auto& key : filenames) {
        std::cerr << "load filename=" << key << std::endl;
        load_file(key);
    }
    is_loaded = true;
    std::cerr << __PRETTY_FUNCTION__ << ": end" << std::endl;
}

size_t settings_t::count(const std::string& key)
{
    load();
    std::cerr << "." << full_name() << " loaded" << std::endl;
    std::cerr << "settings_t::count(" << key << ")" << std::endl;
    try {
        std::cerr << "result: " << std::get<mapping_t>(content_).count(key) << std::endl;
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
