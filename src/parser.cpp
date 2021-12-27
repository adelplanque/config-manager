#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>

#include <boost/algorithm/string/join.hpp>

#include "fmt/format.h"

#include "parser.h"

void dedent(std::list<std::string>& lines)
{
    if (lines.empty()) {
        return;
    }
    int dedent = -1;
    for (auto& line : lines) {
        int spaces = line.find_first_not_of(' ');
        if (dedent == -1 || dedent > spaces) {
            dedent = spaces;
        }
    }
    for (auto& line : lines) {
        line.erase(0, dedent);
    }
}

void comments_t::append(std::list<std::string>&& lines)
{
    dedent(lines);
    if (! this->lines.empty()) {
        this->lines.push_back("");
    }
    this->lines.splice(this->lines.end(), lines);
}

std::string comments_t::format()
{
    std::string result;
    if (this->lines.empty()) {
        return result;
    }
    result = boost::algorithm::join(this->lines, "\n");
    result += "\n";
    return result;
}

std::string option_values_t::format_values()
{
    std::string result;
    if (values.empty()) {
        result = "Unknown key";
        return result;
    }
    if (! values.empty()) {
        for (auto it = values.rbegin(); it != values.rend(); ++it) {
            result += fmt::format("* {} in file {}:\n",
                                  it == values.rbegin() ? "Defined" : "Override",
                                  it->first->native());
            for (const auto& [config, value] : it->second) {
                result.append(fmt::format("  * {}{} = {}\n",
                                          name,
                                          config.empty() ? "" : fmt::format("[{}]", config),
                                          value));
            }
            if (&*it != &values.front()) {
                result += "\n";
            }
        }
    }
    return result;
}

void option_values_t::add_value(const std::shared_ptr<std::filesystem::path>& filename,
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
    values.push_front({ filename, { { config, value } } });
}

const std::string& option_values_t::value(const std::string& config) const
{
    for (auto& [filename, config_values] : values) {
        std::string key = config;
        while (true) {
            const auto it = config_values.find(key);
            if (it != config_values.end()) {
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

// #include <queue>

class file_queue_t
{
public:
    file_queue_t(const std::filesystem::path& filename)
        : is(filename)
    {}

    bool getline(std::string& line) {
        if (q.empty()) {
            return static_cast<bool>(std::getline(is, line));
        } else {
            line = std::move(q.front());
            q.pop_front();
            return true;
        }
    }

    void rollback(std::string&& line) {
        q.push_back(std::move(line));
    }


private:
    std::ifstream is;
    std::list<std::string> q;

};

const std::regex parser_t::group_regex {R"(^\s*\[([_a-zA-Z]\w*)\]\s*$)"};
const std::regex parser_t::comment_regex {R"(^\s*[;\#](.*)$)"};
const std::regex parser_t::option_regex {
    R"(^\s*([_a-zA-Z]\w*)(\[([a-zA-Z]\w*)\])?\s*=\s*(.*)$)" };
const std::regex parser_t::empty_regex {R"(^\s*$)"};

void parser_t::load_file(std::shared_ptr<std::filesystem::path> filename)
{
    std::cerr << "parser_t::load_file(" << *filename << ")" << std::endl;
    file_queue_t infile { *filename };
    // std::ifstream is { *filename };
    std::string line;
    int lineno = 0;
    std::string group_name = "DEFAULT";
    std::smatch m;
    std::list<std::string> comments;

    while (infile.getline(line)) {
        lineno++;
        if (std::regex_match(line, empty_regex)) {
            continue;
        }
        if (std::regex_match(line, m, group_regex)) {
            auto& [group_comments, group_map] = this->options[m[1]];
            if (! comments.empty()) {
                group_comments.append(std::move(comments));
            }
            group_name = std::move(m[1]);
            continue;
        }
        if (std::regex_match(line, m, comment_regex)) {
            comments.push_back(std::move(m[1]));
            continue;
        }
        if (std::regex_match(line, m, option_regex)) {
            auto& [group_comment, group_map] = options[group_name];
            std::string option_name = m[1];
            std::string config_name = m[3];
            std::string value = m[4];
            auto [it, inserted] = group_map.try_emplace(option_name, comments_t(), option_name);
            auto& [option_comments, values] = it->second;
            size_t indent = line.find_first_not_of(' ');
            while (infile.getline(line)) {
                if (line.find_first_not_of(' ') > indent) {
                    value += '\n';
                    value += line;
                } else {
                    infile.rollback(std::move(line));
                    break;
                }
            }
            values.add_value(filename, config_name, value);
            if (! comments.empty()) {
                option_comments.append(std::move(comments));
            }
            continue;
        }
        throw std::runtime_error(fmt::format("Error at {}:{}:\n\t{}\n", filename->native(), lineno, line));
    }
}
