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
#warning Do not include this file directly; include "datastructs.hh" instead
#else


namespace cm {

///
/// A fixed-capacity stack data structure.
/// @tparam T Element type
/// @tparam Capacity Maximum number of elements the stack can hold
///
template<typename T, usize Capacity = ARRAY_LENGTH_UNSPECIFIED>
class FixedStack : private Array<T, Capacity> {
    using Base = Array<T, Capacity>;
    usize _size = 0;

public:
    constexpr FixedStack() noexcept = default;

    ///
    /// Constructor from initializer list
    ///
    constexpr FixedStack(std::initializer_list<T> const& init)
        : Base(init), _size(init.size())
    {}

    ///
    /// Pushes an element onto the stack. If the stack is full, returns false. Otherwise, returns true.
    ///
    constexpr bool push(T const& value)
    {
        if (_size >= Capacity) {
            return false;
        }
        Base::operator()(_size) = value;
        ++_size;
        return true;
    }

    ///
    /// Pops an element from the stack. If the stack is empty, returns None. Otherwise, returns the element that was at
    /// the top of the stack prior to pop().
    ///
    constexpr Optional<T> pop() noexcept
    {
        if (_size == 0) {
            return None;
        }
        --_size;
        return Base::operator()(_size);
    }

    ///
    /// Returns the element at the top of the stack without removing it. If the stack is empty, returns None.
    ///
    constexpr Optional<T> top() const noexcept
    {
        if (_size == 0) {
            return None;
        }
        return Base::operator()(_size - 1);
    }

    ///
    /// Returns the number of elements currently in the stack.
    ///
    constexpr usize length() const noexcept { return _size; }
};

}  // namespace cm
#endif
