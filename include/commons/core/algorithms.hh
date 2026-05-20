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
#include "commons/core/array.hh"
#include <commons/core/typeTraits.hh>
#include <commons/core/intMath.hh>
#include <commons/core/optional.hh>


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

template<typename T>
constexpr inline auto DefaultSortPredicate = [](T const& x, T const& y) -> int {
    if (x < y) {
        return -1;
    } else if (x == y) {
        return 0;
    } else {
        return 1;
    }
};

template<typename T>
constexpr inline auto Ascending = DefaultSortPredicate<T>;

template<typename T>
constexpr inline auto DefaultComparator = [](T const& a, T const& b) -> bool {
    return a == b;
};

template<typename T>
struct SearchResult
{
    i64 index;
    T value;
    SearchResult(i64 index, T value)
        : index(index),
          value(value)
    {}
};

template<typename T>
SearchResult(i64, T) -> SearchResult<T>;

/**
 * @brief Uses linear search to find an occurence of *value* in *container*.
 * @param container The container
 * @param value The value
 * @param cmp A comparator, operator== by default
 * @return The index if found, otherwise -1
 */
template<typename T, typename F = decltype(DefaultComparator<T>)>
Optional<SearchResult<T>> linearSearch(auto const& container, T const& value, F cmp = DefaultComparator<T>)
{
    long i = 0;
    for (auto const& cont_value : container) {
        if (cmp(cont_value, value)) {
            return SearchResult(i, cont_value);
        }
        i++;
    }
    return None;
}


/**
 * @brief Uses binary search to find an occurence of *value* in *container*.
 * @return The index of the target element if found, otherwise -1
 * @param value The value
 * @param cmp A sorting comparison predicate,
 * for a given (x, y) should return a negative integer if x < y, zero if x == y, and a positive integer if x > y.
 * @note
 https://research.google/blog/extra-extra-read-all-about-it-nearly-all-binary-searches-and-mergesorts-are-broken/
 */
template<typename T, typename F = decltype(DefaultComparator<T>)>
Optional<SearchResult<T>> binary(T const& value, ArrayRef<T> const& container, F cmp = DefaultComparator<T>)
{
    i64 low = 0;
    i64 high = i64(container.length()) - 1;

    while (low <= high) {
        // Calculate mid to prevent potential integer overflow
        i64 mid = (low + (high - low) / 2);

        if (cmp(container[mid], value) == 0) {
            return SearchResult(mid, container[mid]);
        } else if (cmp(container[mid], value) < 0) {
            low = mid + 1;  // Target is in the right half
        } else {
            high = mid - 1;  // Target is in the left half
        }
    }
    return None;
}
}  // namespace cm
