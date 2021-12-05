#include <filesystem>
#include <list>
#include <map>
#include <stdexcept>
#include <string>


using string_t = std::string;


class value_t {

    using data_t = std::map<string_t, string_t>;

public:
    value_t() : data {{"", ""}} {};

    const string_t& get(string_t conf_name)
    {
        while (true) {
            const auto it = data.find(conf_name);
            if (it != data.end()) {
                return data.at(conf_name);
            }
            if (conf_name.empty()) {
                throw std::out_of_range("Not found");
            }
            int pos = conf_name.find_last_of("_");
            if (pos == std::string::npos) {
                conf_name.erase();
            } else {
                conf_name.erase(pos);
            }
        }
    }

    ~value_t() {};

private:
    data_t data;

};


using paths_t = std::list<std::filesystem::path>;


class node_t
{
    using node_map_t = std::map<string_t, node_t>;

    enum value_t {null, node_map, node_leaf};
    union node_value_t
    {
        nullptr_t *node_null;
        node_map_t *node_map;
        string_t *node_leaf;

        node_value_t()
        {
            node_null = nullptr;
        }
    };


public:

    node_t() : node_t(nullptr) {};

    node_t(node_t *parent)
        : node_type(value_t::null)
        , node_value()
        , parent(parent)
    {};

    node_t& operator[](const string_t& name)
    {
        switch (node_type)
        {
        case value_t::null:
            node_value.node_map = new node_map_t();
            break;
        case value_t::node_leaf:
            throw std::out_of_range("Leaf !!!");
        default:
            break;
        }
        if (node_value.node_map->find(name) == node_value.node_map->end()) {
        } else {
            return (*node_value.node_map)[name];
        }
    }

    // paths_t get_



private:
    value_t node_type;
    node_value_t node_value;
    node_t *parent;

    void parse_file(const std::list<std::filesystem::path>& files, node_map_t& map) {

    }

};


class settings_t
{
public:
    virtual settings_t& operator[](const string_t& key) = 0;
    virtual ~settings_t() = 0;
};


class settings_node_t : public settings_t
{
public:
    settings_node_t(const std::string& name, settings_node_t* parent)
        : path(parent ? (parent->path / name) : std::filesystem::path(name))
    {};

    virtual settings_t& operator[](const string_t& key)
    {
        if (data.find(key) != data.end())
        {
            return data[key];
        }

    }
    virtual ~settings_node_t() {};

public:
    std::map<string_t, settings_t> data;
    std::filesystem::path path;
};


class settings_leaf_t : public settings_t
{
public:
    settings_leaf_t(const string_t& data) : data(data) {};
    virtual const settings_t& operator[](const string_t& key)
    {
        throw std::out_of_range("Leaf object");
    }
    virtual ~settings_leaf_t() {};

private:
    string_t data;
};


int main(int argc, char** argv)
{
}
