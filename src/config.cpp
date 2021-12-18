#include <vector>
#include <cstdlib>

#include "config.h"

#include <boost/algorithm/string.hpp>

#include <iostream>


config_t* config_t::instance = nullptr;


std::string env_to_string(const char* name)
{
    std::string result;
    char* raw = std::getenv("CONFIG_PATH");
    if (raw != nullptr) {
        result = raw;
    }
    return result;
}


config_t::config_t()
{
    std::vector<std::string> paths;
    for (const auto& path: boost::split(paths, env_to_string("CONFIG_PATH"),
                                        boost::is_any_of(":"))) {
        if (! path.empty()) {
            config_path.push_back(path);
        }
    }
};


void config_t::append_config_path(const std::string& path)
{
    std::cerr << "Append path: " << path << std::endl;
    config_path.push_front(path);
}
