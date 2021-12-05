#include <filesystem>
#include <format>
#include <map>
#include <stdexcept>
#include <string>
#include <variant>


typedef std::string value_t;


class settings_t
{

    typedef std::map<std::string, settings_t> mapping_t;
    typedef std::variant<mapping_t, value_t> content_t;

public:
    settings_t()
        : parent_(nullptr)
        , content_(mapping_t())
    {};
    settings_t(const std::string name, settings_t* parent=nullptr)
        : name_(name)
        , parent_(parent)
        , content_(mapping_t())
    {};
    settings_t(const std::string name, value_t value, settings_t* parent=nullptr)
        : name_(name)
        , parent_(parent)
        , content_(value)
    {};

    const settings_t& at(const std::string key)
    {
        try {
            mapping_t& mapping = std::get<mapping_t>(content_t);
        }
        catch (const std::bad_variant_access& e) {
            throw std::range_error(std::format("Key %s undefined") % e.what());
        }
    }

    std::string full_name()
    {
        std::string name = (parent_t == nullptr) ? "" : parent_t->full_name();
        if (! name_.empty()) {
            name.append(".");
            name.append(name_);
        }
        return name;
    }


private:
    std::string name_;
    settings_t* parent_;
    content_t content_;

};
