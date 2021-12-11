#include <CLI/CLI.hpp>
#include <filesystem>
#include <list>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "fmt/format.h"

#include "parser.h"


typedef std::string value_t;


std::list<std::filesystem::path> config_search_paths = {
    std::filesystem::path("/home/alain/synopsis/config-manager/tests/config-local"),
    std::filesystem::path("/home/alain/synopsis/config-manager/tests/config-global")
};

std::string config_name = "OPER";



class settings_t : public std::enable_shared_from_this<settings_t>
{
public:
    typedef std::shared_ptr<settings_t> settings_ptr;

private:
    typedef std::map<std::string, settings_ptr> mapping_t;
    // typedef std::variant<mapping_t, value_t> content_t;
    typedef std::optional<std::variant<std::mapping_t, value_t>> content_t;

public:
    settings_t()
        : parent_(nullptr)
        , content_(mapping_t())
    {};
    settings_t(const std::string name, settings_ptr parent=nullptr)
        : name_(name)
        , parent_(parent)
        , content_(mapping_t())
    {};
    settings_t(const std::string name, value_t value, settings_ptr parent=nullptr)
        : name_(name)
        , parent_(parent)
        , content_(value)
    {};

    settings_ptr at(const std::string& key)
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

    void set(const std::string& key, const value_t& value) {

    }

    template<typename T>
    T as()
    {
        try {
            return boost::lexical_cast<T>(std::get<value_t>(content_));
        }
        catch (const std::bad_variant_access& ex) {
            throw std::range_error(fmt::format("Key error: {}", full_name()));
        }
    }

    std::string full_name()
    {
        std::string name = (parent_ == nullptr) ? "" : parent_->full_name();
        if (! name_.empty()) {
            name.append(".");
            name.append(name_);
        }
        return name;
    }

    std::out_of_range out_of_range(const std::string& key) {
        return std::out_of_range(fmt::format("Key {} undefined in .{}", key, full_name()));
    }

    std::filesystem::path get_path()
    {
        std::filesystem::path p =
            (parent_ == nullptr) ? std::filesystem::path() : parent_->get_path();
        if (! name_.empty()) {
            p /= name_;
        }
        return p;
    }

    settings_ptr load_file(const std::string& key)
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

        settings_ptr settings { new settings_t(key, shared_from_this()) };
        mapping_t& mapping = std::get<mapping_t>(content_);
        for (auto const& [option_key, option_value] : parser.get_options()) {
            settings_ptr group_settings;
            try {
                std::cout << "group: " << option_key.first << std::endl;
                std::cout << "option: " << option_key.second << std::endl;
                group_settings = mapping.at(option_key.first);
            }
            catch (const std::out_of_range&) {
                mapping[option_key.first]
                    = group_settings
                    = std::make_shared<settings_t>(option_key.first, settings);
            }
            std::get<mapping_t>(content_)
                .emplace(option_key.second,
                         new settings_t(option_key.second,
                                        option_value->value(config_name),
                                        shared_from_this()));
        }

        return settings;
    }

    const settings_ptr& operator[](const std::string& key)
    {
        try {
            mapping_t& mapping = std::get<mapping_t>(content_);
            if (! mapping.count(key)) {
                mapping.emplace(key, std::make_shared<settings_t>(key, shared_from_this()));
            }
            return mapping.at(key);
        }
        catch (const std::bad_variant_access& ex) {
            throw out_of_range(key);
        }
    }

    const settings_ptr& operator=(value_t& value) {

    }


private:
    std::string name_;
    settings_ptr parent_;
    content_t content_;

};


std::string get_key(const std::string key) {
    settings_t::settings_ptr settings(new settings_t());
    settings_t::settings_ptr node = settings;
    std::vector<std::string> toks {};
    for (const auto tok: boost::split(toks, key, boost::is_any_of("."))) {
        std::cout << "node: " << node << std::endl;
        node = node->at(tok);
    }
    return node->as<std::string>();
}

int main(int argc, char** argv)
{
    CLI::App app{"Manage configuration files"};

    std::string key;

    auto get_subcommand = app.add_subcommand("get", "Get config key");
    get_subcommand->add_option("key", key, "Required key");
    get_subcommand->final_callback([&key]() {
        std::cout << get_key(key) << std::endl;
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
