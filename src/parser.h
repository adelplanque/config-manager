#ifndef __parser_h__
#define __parser_h__

#include <filesystem>
#include <memory>
#include <regex>
#include <string>

#include <iostream>


class option_metadata_t
{
public:
    typedef std::shared_ptr<option_metadata_t> ptr;

private:
    typedef std::map<std::string, std::string> config_value_t;

public:
    option_metadata_t() {};

    void add_value(const std::string& config, const std::string& value) {
        std::cout << "add_value: config=" << config << ", value=" << value << std::endl;
        values.emplace(config, value);
    };

    const std::string& value(const std::string& config) {
        std::string key = config;
        while (true) {
            const auto it = values.find(key);
            if (it != values.end()) {
                return values.at(key);
            }
            if (key.empty()) {
                throw std::out_of_range("Not found");
            }
            int pos = key.find_last_of("_");
            if (pos == std::string::npos) {
                key.erase();
            } else {
                key.erase(pos);
            }
        }
    };

private:
    config_value_t values;
    std::string doc;

};


class parser_t
{
public:
    typedef std::shared_ptr<parser_t> ptr;
    typedef std::pair<std::string, std::string> option_key_t;
    typedef std::map<option_key_t, option_metadata_t::ptr> options_t;

    parser_t() {};
    void load_file(const std::filesystem::path& filename);
    bool empty() { return options.empty(); }
    const options_t& get_options() { return options; };

private:

    static const std::regex group_regex;
    static const std::regex comment_regex;
    static const std::regex option_regex;
    static const std::regex empty_regex;

    options_t options;
};

#endif
