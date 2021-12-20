#ifndef __env_reflector_h__
#define __env_reflector_h__

#include "env.h"

namespace jinja2
{
namespace detail
{
    class EnvironnementMapAccessor
        : public MapItemAccessor
        , public ReflectedDataHolder<environnement_t>
    {
    public:
        using ReflectedDataHolder<environnement_t>::ReflectedDataHolder;
        ~EnvironnementMapAccessor() override = default;

        size_t GetSize() const override
        {
            return this->GetValue()->size();
        }

        bool HasValue(const std::string& name) const override
        {
            return this->GetValue()->count(name);
        }

        Value GetValueByName(const std::string& name) const override
        {
            return this->GetValue()->at(name);
        }

        std::vector<std::string> GetKeys() const override
        {
            return this->GetValue()->keys();
        }

    };

    template<>
    struct Reflector<environnement_t>
    {
        static Value Create(environnement_t env)
        {
            return GenericMap([accessor = EnvironnementMapAccessor(env)]() {
                    return &accessor;
                });
        }
    };
}
}

#endif
