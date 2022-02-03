// Copyright (C) 2022 Alain Delplanque

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <fstream>

#include <boost/algorithm/string.hpp>

#include <CLI/CLI.hpp>
#include <fmt/format.h>
#include <jinja2cpp/string_helpers.h>

#include "settings.h"
#include "config.h"
#include "render.h"


std::string get_key(const std::string key)
{
    settings_t::settings_ptr settings(new settings_t());
    settings_t::settings_ptr node = settings;
    std::vector<std::string> toks {};
    for (const auto tok: boost::split(toks, key, boost::is_any_of("."))) {
        std::cerr << "node: " << node << std::endl;
        node = node->at(tok);
    }
    return node->as<std::string>();
}

void doc_key(const std::string& key)
{
    settings_t::settings_ptr settings(new settings_t());
    settings_t::settings_ptr node = settings;
    std::vector<std::string> toks {};
    for (const auto tok: boost::split(toks, key, boost::is_any_of("."))) {
        std::cerr << "node: " << node << std::endl;
        node = node->at(tok);
    }
    node->doc(std::cout);
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
    app.add_option("--config-name", config_t::get_instance().get_config_name(),
                   "Configuration name");

    auto get_subcommand = app.add_subcommand("get", "Get config key");
    get_subcommand->add_option("key", key, "Required key");
    get_subcommand->final_callback([&key]() {
        std::cout << get_key(key) << std::endl;
    });

    auto render_subcommand = app.add_subcommand("render", "Render template file");
    render_subcommand->add_option("filename", template_filename, "Template file");
    render_subcommand->final_callback([&template_filename]() {
        std::ifstream is { template_filename };
        try {
            std::cout << render(std::make_shared<settings_t>(), is);
        }
        catch (nonstd::expected_lite::bad_expected_access<jinja2::ErrorInfo>& e)
        {
            std::cerr << fmt::format("Template error at {}:{}",
                                     template_filename, e.error().GetErrorLocation().line)
                      << e.error() << std::endl;
        }
    });

    auto help_subcommand = app.add_subcommand("doc", "Documentation for given key");
    help_subcommand->add_option("key", key, "Required key");
    help_subcommand->final_callback([&key]() { doc_key(key); });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
