#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>

#include "fmt/format.h"

#include "parser.h"


void option_metadata_t::add_value(std::shared_ptr<std::filesystem::path> filename,
                                  const std::string& config, const std::string& value)
{
    std::cerr << "option_metadata_t::add_value("
              << *filename << ", "
              << config << ", "
              << value << ")" << std::endl;
    for (auto& [exist_filename, config_values] : values) {
        if (exist_filename == filename) {
            config_values[config] = value;
            return;
        }
    }
    values.push_back({ filename, { { config, value } } });
}

const std::string& option_metadata_t::value(const std::string& config) const
{
    std::cerr << "option_metadata_t::value(" << config << ")" << std::endl;
    for (auto& [filename, config_values] : values) {
        std::string key = config;
        while (true) {
            const auto it = config_values.find(key);
            if (it != config_values.end()) {
                std::cerr << "value found in file: " << *filename << std::endl;
                return config_values.at(key);
            }
            if (key.empty()) {
                break;
            }
            size_t pos = key.find_last_of("_");
            if (pos == std::string::npos) {
                key.erase();
            } else {
                key.erase(pos);
            }
        }
    }
    throw std::out_of_range("Not found");
}

const std::regex parser_t::group_regex {R"(^\s*\[([_a-zA-Z]\w*)\]\s*$)"};
const std::regex parser_t::comment_regex {R"(^\s*[;\#](.*)$)"};
const std::regex parser_t::option_regex {
    R"(^\s*([_a-zA-Z]\w*)(\[([a-zA-Z]\w*)\])?\s*=\s*(.*)$)" };
const std::regex parser_t::empty_regex {R"(^\s*$)"};

void parser_t::load_file(std::shared_ptr<std::filesystem::path> filename)
{
    std::cerr << "parser_t::load_file(" << *filename << ")" << std::endl;
    std::ifstream is { *filename };
    std::string line;
    int lineno = 0;
    std::string group { "default" };
    std::smatch m;
    while (getline(is, line)) {
        lineno++;
        if (std::regex_match(line, empty_regex)) {
            continue;
        }
        if (std::regex_match(line, m, group_regex)) {
            group = m[1];
            continue;
        }
        if (std::regex_match(line, m, comment_regex)) {
            continue;
        }
        if (std::regex_match(line, m, option_regex)) {
            option_metadata_t::ptr option;
            option_key_t key { group, m[1] };
            try {
                option = options.at(key);
            }
            catch (std::out_of_range&) {
                options[key] = option = std::make_shared<option_metadata_t>();
            }
            option->add_value(filename, m[3], m[4]);
            continue;
        }
        throw std::runtime_error(fmt::format("Error at {}:{}:\n\t{}\n", filename->native(), lineno, line));
    }
}
