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

///
/// Defines common functionality for anything that is an iterable (i.e. a collection of elements with an iterator)
/// This massively reduces the amount of repeated code and ensures each collection (Array, String, Linked list, etc.)
/// has the same functionality.
///
template<typename Derived>
struct Iterable
{
    ///
    /// Generates a string representation of an Iterable. Outputs (within curly brackets) the string representation of
    /// each element in the array, delimited by commas.
    ///
    constexpr static void outputString(Iterable const& self, auto const& out)
    {
        out('{');
        auto const& _self = DerivedRef(self);
        if (_self.length() != 0) {
            auto iter = _self.begin();
            auto i = 0uz;
            while (i < _self.length() - 1) {
                auto const& value = *iter;
                ::cm::OutputString(value, out);
                out(',');
                out(' ');
                ++i;
                ++iter;
            }
            if (i < _self.length()) {
                auto const& value = *iter;
                ::cm::OutputString(value, out);
            }
        }
        out('}');
    }

    ///
    /// Performs a deep equality comparison of two arrays.
    ///
    constexpr bool equals(this Iterable const& self, auto const& other)
    {
        auto const& _self = DerivedRef(self);
        auto const& otherDerived = DerivedRef(other);

        if (&self == &other) {
            return true;
        }
        if (_self.length() != otherDerived.length()) {
            return false;
        }
        auto x = _self.begin();
        auto y = otherDerived.begin();
        while (x != _self.end()) {
            if (*x != *y) {
                return false;
            }
            ++x;
            ++y;
        }
        return true;
    }

    ///
    /// Performs an equality comparison of two iterables that **attempts** to mitigate timing vulnerabilities.
    /// Generally slower than equals(), but tries to ensure that comparison always takes the same amount of time for a
    /// given length.
    ///
    constexpr int equalsTimesafe(this Iterable const& self, auto const& other)
    {
        auto const& _self = DerivedRef(self);
        auto const& otherDerived = DerivedRef(other);
        auto selfIter = _self.begin();
        auto otherIter = otherDerived.begin();
        auto remaining = i64(::cm::max(_self.length(), otherDerived.length()));
        auto i = i64(0);
        auto notEqual = 0;

        while (i < i64(::cm::min(_self.length(), otherDerived.length()))) {
            notEqual |= (*selfIter != *otherIter);
            ++selfIter;
            ++otherIter;
            ++i;
            --remaining;
        }
        return !notEqual && (remaining == 0);
    }

    ///
    /// Performs a deep comparison of the elements in two iterables.
    ///
    constexpr int compare(this Iterable const& self, auto const& other)
    {
        auto const& _self = DerivedRef(self);
        auto const& otherDerived = DerivedRef(other);

        if (&self == &other) {
            return 0;
        }
        if (_self.length() != otherDerived.length()) {
            return Compare(_self.length(), otherDerived.length());
        }
        auto x = _self.begin();
        auto y = otherDerived.begin();
        while (x != _self.end()) {
            if (*x < *y) {
                return -1;
            } else if (*x > *y) {
                return 1;
            }
            ++x;
            ++y;
        }
        return 0;
    }

    ///
    /// Performs a deep comparison of two iterables that **attempts** to mitigate timing vulnerabilities.
    /// Generally slower than compare(), but tries to ensure that comparison always takes the same amount of time for a
    /// given length.
    ///
    constexpr int compareTimesafe(this Iterable const& self, auto const& other)
    {
        auto const& _self = DerivedRef(self);
        auto const& otherDerived = DerivedRef(other);
        auto selfIter = _self.begin();
        auto otherIter = otherDerived.begin();
        auto remaining = i64(::cm::max(_self.length(), otherDerived.length()));
        auto i = i64(0);
        auto notEqual = 0;
        auto sign = 0;

        while (i < i64(::cm::min(_self.length(), otherDerived.length()))) {
            auto c = Compare(*selfIter, *otherIter);
            notEqual |= (c != 0);
            sign |= (c * (sign == 0));
            ++selfIter;
            ++otherIter;
            ++i;
            --remaining;
        }
        return (notEqual * sign) - !!remaining;
    }

    ///
    /// Returns a reference to the smallest element in the Iterable.
    ///
    constexpr auto const& min(this Iterable const& self)
    {
        auto const& _self = DerivedRef(self);
        auto iter = _self.begin();
        auto const* minValue = &*iter;

        while (iter != _self.end()) {
            if (*iter < *minValue) {
                minValue = &*iter;
            }
            ++iter;
        }
        return *minValue;
    }

    ///
    /// Returns a reference to the largest element in the Iterable.
    ///
    constexpr auto const& max(this Iterable const& self)
    {
        auto const& _self = DerivedRef(self);
        auto iter = _self.begin();
        auto const* maxValue = &*iter;

        while (iter != _self.end()) {
            if (*iter > *maxValue) {
                maxValue = &*iter;
            }
            ++iter;
        }
        return *maxValue;
    }

    /// Returns a product of all the elements.
    /// @tparam SumType the type which the sum is stored in. This is useful, for example, if you have an array
    /// of 8-bit integers and you want to store the sum in a 64-bit integer. Without a SumType specified, the
    /// sum will be the same type as the elements (in this case, an 8-bit integer), which will overflow and not
    /// store the sum correctly.
    ///
    template<typename SumType = NoneType>
    constexpr auto sum(this Iterable const& self)
    {
        auto const& _self = DerivedRef(self);
        auto iter = _self.begin();

        // Type of the sum value is:
        // if SumType is NoneType: same type as the element
        // otherwise, SumType
        SelectType<IsSame<SumType, NoneType>, SumType, CRefRemoved<decltype(*_self.begin())>> res = *iter;
        ++iter;
        while (iter != _self.end()) {
            res += *iter;
            ++iter;
        }
        return res;
    }

    ///
    /// Returns a product of all the elements.
    /// @tparam ProductType the type which the product is stored in. This is useful, for example, if you have an array
    /// of 8-bit integers and you want to store the product in a 64-bit integer. Without a ProductType specified, the
    /// product will be the same type as the elements (in this case, an 8-bit integer), which will overflow and not
    /// store the product correctly.
    ///
    template<typename ProductType = NoneType>
    constexpr auto product(this Iterable const& self)
    {
        auto const& _self = DerivedRef(self);
        auto iter = _self.begin();
        SelectType<IsSame<ProductType, NoneType>, ProductType, CRefRemoved<decltype(*_self.begin())>> res = *iter;
        ++iter;
        while (iter != _self.end()) {
            res *= *iter;
            ++iter;
        }
        return res;
    }

    ///
    /// Computes the mean (arithmetic average), calculated by dividing the sum by the number of values
    ///
    template<typename MeanType = NoneType>
    constexpr auto mean(this Iterable const& self)
    {
        using T = SelectType<IsSame<MeanType, NoneType>, MeanType, double>;
        return self.sum<T>() / T(DerivedRef(self).length());
    }

    ///
    /// Finds the first occurence of a contiguous
    ///
    inline Optional<usize> find(this Iterable const& self, Derived const& str, usize baseIndex = 0)
    {
        auto _self = DerivedRef(self);
        if (_self.length() < str.length()) {
            return None;
        } else if (_self.length() == str.length() && _self.equals(str)) {
            return 0uz;
        } else {
            for (usize i = baseIndex; i < _self.length() - str.length(); i++) {
                UNSAFE_BEGIN;
                auto s1 = _self.slice(i, str.length());
                UNSAFE_END;
                auto s2 = str.slice(0, str.length());
                if (s1.equals(s2)) {
                    return i;
                }
            }
            return None;
        }
    }


private:
    using DerivedRef = Derived const&;
};

}  // namespace cm
#endif
