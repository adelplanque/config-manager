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

#include <list>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include <jinja2cpp/reflected_value.h>
#include <jinja2cpp/template.h>
#include <jinja2cpp/user_callable.h>
#include <subprocess.hpp>

#include "env.h"
#include "env_reflector.h"
#include "render.h"
#include "settings.h"

std::string shell(const std::string& script)
{
    std::string result;
    try {
        auto process =
            subprocess::run({"bash", "-c", script},
                            subprocess::RunBuilder()
                            .cout(subprocess::PipeOption::pipe)
                            .cerr(subprocess::PipeOption::pipe)
                            .check(true));
        result = std::move(process.cout);
        boost::algorithm::trim(result);
    }
    catch (const subprocess::CalledProcessError& e) {
        result = "FAIL!";
    }
    return result;
}


class Renderer;

struct SettingsProxy
{
    // SettingsProxy(settings_t::settings_ptr settings, settings_t::settings_ptr root,
    //               Renderer& renderer)
    //     : settings(settings)
    //     , root(root)
    //     , renderer(renderer)
    // {};

    settings_t::settings_ptr settings;
    settings_t::settings_ptr root;
    Renderer& renderer;
};


namespace jinja2
{
namespace detail
{
    class SettingsMapAccessor : public MapItemAccessor
                              , public ReflectedDataHolder<SettingsProxy>
    {
    public:
        using ReflectedDataHolder<SettingsProxy>::ReflectedDataHolder;
        ~SettingsMapAccessor() override = default;

        size_t GetSize() const override
        { return this->GetValue()->settings->size(); }

        bool HasValue(const std::string& name) const override
        { return this->GetValue()->settings->count(name); }

        Value GetValueByName(const std::string& name) const override
        {
            auto val = this->GetValue();
            SettingsProxy proxy {val->settings->at(name), val->root, val->renderer };
            return Reflect(proxy);
        }

        std::vector<std::string> GetKeys() const override
        { return this->GetValue()->settings->keys(); }
    };

    template<>
    struct Reflector<SettingsProxy>
    {
        static Value Create(SettingsProxy& proxy)
        {
            std::cerr << __PRETTY_FUNCTION__ << ": " << proxy.settings->full_name() << std::endl;
            Value result;
            switch (proxy.settings->type())
            {
            case settings_t::type_t::mapping:
                result = GenericMap([accessor = SettingsMapAccessor(proxy)]() {
                    return &accessor;
                });
                break;
            case settings_t::type_t::value:
                auto value = proxy.settings->as<std::string>();
                if (value.find("{{") != std::string::npos
                    || value.find("{%") != std::string::npos)
                {
                    proxy.renderer.push_back(proxy.settings->full_name());
                    value = proxy.renderer.render(value);
                    proxy.renderer.pop();
                }
                std::cerr << __PRETTY_FUNCTION__ << ": " << proxy.settings->full_name() << " => " << value << std::endl;
                result = value;
                break;
            }
            return result;
        }
    };
}
}

Renderer::Renderer(settings_t::settings_ptr settings)
{
    SettingsProxy proxy {settings, settings->get_root(), *this};
    values = jinja2::ValuesMap ({
                {"env", jinja2::Reflect(environnement_t())},
                {"settings", jinja2::Reflect(proxy)},
                {"shell", jinja2::MakeCallable(shell, jinja2::ArgInfo("script", true))},
        });
}

std::string Renderer::render(std::istream& is)
{
    jinja2::Template tpl;
    tpl.Load(is).value();
    settings_t::settings_ptr settings { new settings_t() };
    return tpl.RenderAsString(this->values).value();
}

std::string render(settings_t::settings_ptr settings, std::istream& is)
{
    return Renderer(settings).render(is);
}
