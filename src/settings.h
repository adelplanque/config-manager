#ifndef __settings_h__
#define __settings_h__

#include <map>
#include <memory>
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
    value_t(const std::string& value, option_metadata_t::ptr metadata)
        : value(value)
        , metadata(metadata)
    {}
    value_t(value_t&& other)
        : value(std::move(other.value))
        , metadata(other.metadata)
    {
        std::cerr << "value_t move contructor" << std::endl;
    }

    template<typename T> T as() { return boost::lexical_cast<T>(value); }
    std::string doc() { return metadata ? metadata->format_doc() : ""; }

private:
    std::string value;
    option_metadata_t::ptr metadata;
};


class settings_t : public std::enable_shared_from_this<settings_t>
{
public:
    typedef std::shared_ptr<settings_t> settings_ptr;
    enum type_t {mapping, value};

private:
    typedef std::map<std::string, settings_ptr> mapping_t;
    typedef std::variant<mapping_t, value_t> content_t;
    // typedef std::optional<std::variant<mapping_t, value_t>> content_t;

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
    void set_value(const std::string& key, value_t&& value);
    size_t count(const std::string& key);
    std::vector<std::string> keys();
    void doc(std::ostream& os);

    template<typename T> T as()
    {
        try {
            return std::get<value_t>(content_).as<T>();
        }
        catch (const std::bad_variant_access& ex) {
            throw std::out_of_range(fmt::format("Key error: {}", full_name()));
        }
    }

    type_t type()
    {
        if (std::holds_alternative<mapping_t>(content_)) {
            return mapping;
        } else {
            return value;
        }
    }

    std::string full_name();
    std::out_of_range out_of_range(const std::string& key);
    std::filesystem::path get_path();

private:
    std::string name_;
    settings_ptr parent_;
    content_t content_;
    bool is_loaded;

    void load();
    void load_file();
};

#endif
