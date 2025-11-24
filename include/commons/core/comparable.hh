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
#include <commons/core.hh>

namespace cm {

/*
 Typeless predicate for comparing.
 Should return a negative integer if the first element is less than the second;
 A positive integer if the first element is greater than the second;
 Zero if the elements are equal.
*/
using Comparator = int (*)(void const*, void const*);

/*
 Comparable interface. Derived must implement: int compare(Derived)-
 which returns 0 if equal, 1 if greater than, -1 if less than.
*/
template<typename Derived>
struct IComparable
{

    constexpr static Comparator comparator = [](void const* e1, void const* e2) -> int {
        auto t1 = reinterpret_cast<Derived const*>(e1);
        auto t2 = reinterpret_cast<Derived const*>(e2);
        return t1->compare(*t2);
    };

    constexpr inline bool operator<(auto x) const { return static_cast<Derived const*>(this)->compare(x) < 0; }

    constexpr inline bool operator>(auto x) const { return static_cast<Derived const*>(this)->compare(x) > 0; }

    constexpr inline bool operator<=(auto x) const { return static_cast<Derived const*>(this)->compare(x) <= 0; }

    constexpr inline bool operator>=(auto x) const { return static_cast<Derived const*>(this)->compare(x) >= 0; }

    constexpr inline bool lessThanAny(auto... args) const { return ((this->operator<(args)) || ...); }

    constexpr inline bool lessThanAll(auto... args) const { return ((this->operator<(args)) && ...); }

    constexpr inline bool greaterThanAny(auto... args) const { return ((this->operator>(args)) || ...); }

    constexpr inline bool greaterThanAll(auto... args) const { return ((this->operator>(args)) && ...); }

    constexpr inline bool lessEqualThanAny(auto... args) const { return ((this->operator<=(args)) || ...); }

    constexpr inline bool lessEqualThanAll(auto... args) const { return ((this->operator<=(args)) && ...); }

    constexpr inline bool greaterEqualThanAny(auto... args) const { return ((this->operator>=(args)) || ...); }

    constexpr inline bool greaterEqualThanAll(auto... args) const { return ((this->operator>=(args)) && ...); }
};

template<typename A, typename B = A>
concept IsFullyComparable = requires (A a, B b) {
    {
        a == b
    } -> IsSame<bool>;
    {
        a != b
    } -> IsSame<bool>;
    {
        a < b
    } -> IsSame<bool>;
    {
        a <= b
    } -> IsSame<bool>;
    {
        a > b
    } -> IsSame<bool>;
    {
        a >= b
    } -> IsSame<bool>;
};

}  // namespace cm
