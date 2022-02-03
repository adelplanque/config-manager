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
