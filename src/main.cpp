#include <CLI/CLI.hpp>

#include "settings.h"


std::string get_key(const std::string key) {
    settings_t::settings_ptr settings(new settings_t());
    settings_t::settings_ptr node = settings;
    std::vector<std::string> toks {};
    for (const auto tok: boost::split(toks, key, boost::is_any_of("."))) {
        std::cout << "node: " << node << std::endl;
        node = node->at(tok);
    }
    return node->as<std::string>();
}

int main(int argc, char** argv)
{
    CLI::App app{"Manage configuration files"};

    std::string key;

    auto get_subcommand = app.add_subcommand("get", "Get config key");
    get_subcommand->add_option("key", key, "Required key");
    get_subcommand->final_callback([&key]() {
        std::cout << get_key(key) << std::endl;
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
