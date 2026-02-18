/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

#pragma once
#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else

namespace cm {

template<typename Container_>
struct ComputedProperty
{
protected:
    friend Container_;

    ComputedProperty() = default;

    template<typename T1>
    inline auto findOffset(T1 Container_::* member) const noexcept
    {
        signed char data[sizeof(Container_)];
        Container_ const& object = *reinterpret_cast<Container_*>(data);
        return reinterpret_cast<long long>(&(object.*member)) - reinterpret_cast<long long>(&object);
    }

    template<typename T1>
    inline Container_* container(T1 Container_::* member) noexcept
    {
        return reinterpret_cast<Container_*>(reinterpret_cast<long long>(this) - findOffset(member));
    }

    template<typename T1>
    inline Container_ const* container(T1 Container_::* member) const noexcept
    {
        return reinterpret_cast<Container_ const*>(reinterpret_cast<long long>(this) - findOffset(member));
    }
};
}  // namespace cm
#endif
