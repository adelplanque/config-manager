#ifndef __parser_h__
#define __parser_h__

#include <filesystem>
#include <list>
#include <memory>
#include <regex>
#include <string>

#include <iostream>


class option_metadata_t
{
public:
    typedef std::shared_ptr<option_metadata_t> ptr;

private:
    typedef std::map<std::string, std::string> config_values_t;
    typedef std::pair<std::shared_ptr<std::filesystem::path>, config_values_t> filename_values_t;
    typedef std::list<filename_values_t> values_t;
    typedef std::map<std::shared_ptr<std::filesystem::path>, std::list<std::string>> comments_t;

public:
    option_metadata_t(const std::string& name)
        : name(name)
    {};

    void add_value(std::shared_ptr<std::filesystem::path> filename,
                   const std::string& config, const std::string& value);
    const std::string& value(const std::string& config) const;
    void set_comments(std::shared_ptr<std::filesystem::path> filename,
                      std::list<std::string>&& comments) {
        std::cerr << "set_comments " << *filename << std::endl;
        auto& file_comments = this->comments[filename];
        file_comments.splice(file_comments.end(), comments);
    }
    std::string format_doc();

private:
    std::string name;
    values_t values;
    comments_t comments;
};


class parser_t
{
public:
    typedef std::shared_ptr<parser_t> ptr;
    typedef std::pair<std::string, std::string> option_key_t;
    typedef std::map<option_key_t, option_metadata_t::ptr> options_t;

    parser_t() {};
    void load_file(std::shared_ptr<std::filesystem::path> filename);
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
