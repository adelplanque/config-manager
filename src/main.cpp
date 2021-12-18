#include <fstream>

#include <boost/algorithm/string.hpp>

#include <CLI/CLI.hpp>

#include "settings.h"
#include "config.h"
#include "render.h"


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


void append_config_path(const std::string& path) {
    config_t::get_instance().append_config_path(path);
}


int main(int argc, char** argv)
{
    CLI::App app{"Manage configuration files"};

    std::string key;
    std::string template_filename;

    app.add_option_function("--path", std::function<void(const std::string&)>(append_config_path),
                            "Path to configuration files");

    auto get_subcommand = app.add_subcommand("get", "Get config key");
    get_subcommand->add_option("key", key, "Required key");
    get_subcommand->final_callback([&key]() {
        std::cout << get_key(key) << std::endl;
    });

    auto render_subcommand = app.add_subcommand("render", "Render template file");
    render_subcommand->add_option("filename", template_filename, "Template file");
    render_subcommand->final_callback([&template_filename]() {
        std::ifstream is { template_filename };
        render(is);
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
