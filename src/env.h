#ifndef __env_h__
#define __env_h__

#include <string>
#include <vector>


class environnement_t
{
public:
    std::string at(const std::string& name) const;
    size_t size() const;
    bool count(const std::string& name) const;
    std::vector<std::string> keys() const;
};

#endif
