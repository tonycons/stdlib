/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.


   File: algorithm/predicates
   Description: Defines a bunch of predicates to use in filter functions.
*/

#pragma once
#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else


namespace cm {

// Potentially unsafe as pointer deference may be involved
UNSAFE_BEGIN;


constexpr auto Length(auto const& val)
{
    using T = decltype(val);
    if constexpr (IsPointer<T> && IsUnderlyingTypeOneOf<T, char*, wchar_t*, char8_t*, char16_t*, char32_t*>) {
        if (sizeof(*val) < 2) {
            return __builtin_strlen(val);
        } else if (sizeof(*val) == sizeof(wchar_t)) {
            return __builtin_wcslen(val);
        } else {
            unsigned long long len = 0;
            while (*val++ != 0) {
                len++;
            }
            return len;
        }
    } else {
        return val.length();
    }
}

template<typename T, unsigned N>
constexpr auto Length(T const (&)[N])
{
    if constexpr (IsUnderlyingTypeOneOf<T, char, wchar_t, char8_t, char16_t, char32_t>) {
        return max(0L, long(N) - 1L);
    } else {
        return N;
    }
}

constexpr auto FirstElement(auto&& elements)
{
    if constexpr (IsIterable<decltype(elements)>) {
        return *elements.begin();
    } else {
        return elements[0];
    }
}

constexpr auto LastElement(auto&& elements)
{
    if constexpr (IsIterable<decltype(elements)>) {
        return *elements.end();
    } else {
        return elements[Length(elements) - 1];
    }
}

constexpr auto Contains(auto&& elements, auto const& val)
{
    if constexpr (IsIterable<decltype(elements)>) {
        for (auto const& element : elements) {
            if (element == val) {
                return true;
            }
        }
        return false;
    } else {
        for (decltype(Length(elements)) i = {}; i < Length(elements); i++) {
            if (elements[i] == val) {
                return true;
            }
        }
        return false;
    }
}

UNSAFE_END;

namespace Predicate {
template<auto Val>
struct StartsWith
{
    constexpr static bool operator()(auto const& iterable) { return FirstElement(iterable) == Val; }
};
template<auto Val>
struct EndsWith
{
    constexpr static bool operator()(auto const& iterable) { return LastElement(iterable) == Val; }
};
template<auto Val>
struct Contains
{
    constexpr static bool operator()(auto const& iterable, auto const& val) { return Contains(iterable, val); }
};
}  // namespace Predicate


template<auto Val>
constexpr inline Predicate::StartsWith<Val> StartsWith;
template<auto Val>
constexpr inline Predicate::EndsWith<Val> EndsWith;

}  // namespace cm
#endif
