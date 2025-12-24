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
/// An abstraction over array indices that allows negative indices to index from the end of the array like in python
/// Initializing an index with an unsigned integer works as expected
/// Initializing with a negative signed integer creates an index relative to the last element.
/// Index -1 refers to the last element.
///
struct Index : Union<isize, usize>
{
    using Union<isize, usize>::Union;

    ///
    /// Returns an unsigned index into an array. Does not check if the index is in bounds.
    /// @param a The array to compute the index for.
    ///
    NODISCARD FORCEINLINE constexpr auto computeUnchecked(auto const& a) const
    {
        usize i = match(
            [&](usize k) { return k; },
            [&](isize k) {  // Possible negative index
                return k < 0 ? usize(isize(a.length()) + k) : usize(k);
            });
        return i;
    }

    ///
    /// Returns an unsigned index into an array. Checks if the index is in bounds.
    /// @param a The array to compute the index for.
    ///
    NODISCARD FORCEINLINE constexpr auto compute(auto const& a) const
    {
        usize i = computeUnchecked(a);
        Assert(i < a.length(), ASMS_BOUNDS);
        return i;
    }

    ///
    /// Asserts that the index is not negative, and if so, returns the index as an unsigned integer.
    ///
    NODISCARD FORCEINLINE constexpr usize assertPositive() const
    {
        usize i = match(
            [&](usize k) { return k; },
            [&](isize k) {
                Assert(k >= 0, ASMS_PARAMETER);
                return usize(k);
            });
        return i;
    }
};


}  // namespace cm
#endif
