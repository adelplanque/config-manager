#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>

#include "fmt/format.h"

#include "parser.h"


const std::regex parser_t::group_regex {R"(^\s*\[([_a-zA-Z]\w*)\]\s*$)"};
const std::regex parser_t::comment_regex {R"(^\s*[;\#](.*)$)"};
const std::regex parser_t::option_regex {
    R"(^\s*([_a-zA-Z]\w*)(\[([a-zA-Z]\w*)\])?\s*=\s*(.*)$)" };
const std::regex parser_t::empty_regex {R"(^\s*$)"};

void parser_t::load_file(const std::filesystem::path& filename)
{
    std::cout << "parser_t::load_file " << filename << std::endl;
    std::ifstream is {filename};
    std::string line;
    int lineno = 0;
    std::string group { "default" };
    std::smatch m;
    while (getline(is, line)) {
        lineno++;
        std::cout << "Line: " << line << std::endl;
        if (std::regex_match(line, empty_regex)) {
            continue;
        }
        if (std::regex_match(line, m, group_regex)) {
            std::cout << "Group: " << m[1] << std::endl;
            group = m[1];
            continue;
        }
        if (std::regex_match(line, m, comment_regex)) {
            std::cout << "Comment: " << m[1] << std::endl;
            continue;
        }
        if (std::regex_match(line, m, option_regex)) {
            std::cout << "Option: key=" << m[1] << ", config=" << m[3]
                      << ", value=" << m[4] << std::endl;
            option_metadata_t::ptr option;
            option_key_t key { group, m[1] };
            try {
                option = options.at(key);
            }
            catch (std::out_of_range&) {
                options[key] = option = std::make_shared<option_metadata_t>();
            }
            option->add_value(m[3], m[4]);
            continue;
        }
        throw std::runtime_error(fmt::format("Error at {}:{}:\n\t{}\n", filename.native(), lineno, line));
    }
}
