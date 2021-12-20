#include <cstdlib>
#include <unistd.h>

#include "env.h"


std::string environnement_t::at(const std::string& name) const
{
    std::string result;
    const char* value = std::getenv(name.c_str());
    if (value != nullptr) {
        result = value;
    }
    return result;
}

size_t environnement_t::size() const
{
    size_t result = 0;
    while (environ[result]) {
        ++result;
    }
    return result;
}

bool environnement_t::count(const std::string& name) const
{
    const char* value = std::getenv(name.c_str());
    return value != nullptr;
}

std::vector<std::string> environnement_t::keys() const
{
    std::vector<std::string> result;
    for (size_t i = 0; environ[i]; i++) {
        char* end = environ[i];
        while (*end != '\0' && *end != '=') {
            end++;
        }
        result.push_back(std::string(environ[i], end));
    }
    return result;
}
