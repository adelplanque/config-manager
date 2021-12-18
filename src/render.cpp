#include <boost/algorithm/string.hpp>

#include <jinja2cpp/reflected_value.h>
#include <jinja2cpp/template.h>
#include <jinja2cpp/user_callable.h>
#include <subprocess.hpp>

#include "env.h"
#include "env_reflector.h"
#include "render.h"
#include "settings.h"


namespace jinja2
{
namespace detail
{
    class SettingsMapAccessor : public MapItemAccessor
                           , public ReflectedDataHolder<settings_t::settings_ptr>
    {
    public:
        using ReflectedDataHolder<settings_t::settings_ptr>::ReflectedDataHolder;
        ~SettingsMapAccessor() override = default;

        size_t GetSize() const override
        {
            return 0;
        }

        bool HasValue(const std::string& name) const override
        {
            auto val = this->GetValue();
            return (*val)->count(name);
        }

        Value GetValueByName(const std::string& name) const override
        {
            auto val = this->GetValue();
            return Reflect((*val)->at(name));
        }

        std::vector<std::string> GetKeys() const override
        {
            std::cerr << __PRETTY_FUNCTION__ << ": start" << std::endl;
            return (*this->GetValue())->keys();
        }

    };

    template<>
    struct Reflector<settings_t::settings_ptr>
    {
        static Value Create(settings_t::settings_ptr settings)
        {
            Value result;
            switch (settings->type())
            {
            case settings_t::type_t::mapping:
                result = GenericMap([accessor = SettingsMapAccessor(settings)]() {
                    return &accessor;
                });
                break;
            case settings_t::type_t::value:
                result = settings->as<std::string>();
                break;
            }
            return result;
        }
    };
}
}

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

const std::string render(std::istream& is)
{
    jinja2::Template tpl;
    tpl.Load(is);
    settings_t::settings_ptr settings { new settings_t() };
    jinja2::ValuesMap params = {
        {"env", jinja2::Reflect(environnement_t())},
        {"settings", jinja2::Reflect(settings)},
        {"shell", jinja2::MakeCallable(shell, jinja2::ArgInfo("script", true))},
    };
    // try {
    return tpl.RenderAsString(params).value();
    // }
    // catch (const bad_expected_access& e) {

    // }

    return "toto";
}
