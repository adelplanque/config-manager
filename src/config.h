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
        std::cout << "instance: " << config_t::instance << std::endl;
        return *config_t::instance;
    }

    const std::list<std::filesystem::path>& get_config_path() { return config_path; }
    void append_config_path(const std::string& path);

private:
    config_t();
    static config_t* instance;

    std::list<std::filesystem::path> config_path;
};

#endif
