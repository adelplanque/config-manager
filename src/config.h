#ifndef __config_h__
#define __config_h__

#include <list>
#include <filesystem>

#include <iostream>

class config_t {

public:
    static config_t& get_instance()
    {
        if (config_t::instance == nullptr) {
            config_t::instance = new config_t();
        }
        std::cerr << "instance: " << config_t::instance << std::endl;
        return *config_t::instance;
    }

    const std::list<std::filesystem::path>& get_config_path() const { return config_path; }
    std::string& get_config_name() { return config_name; }
    void append_config_path(const std::string& path);

private:
    config_t();
    static config_t* instance;

    std::list<std::filesystem::path> config_path;
    std::string config_name;
};

#endif
