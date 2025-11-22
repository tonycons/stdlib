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
#ifdef __inline_core_header__
#include <sys/types.h>


namespace cm {

///
/// An abstraction over array indices that allows negative indices to index from the end of the array like in python
/// Initializing an index with an unsigned integer works as expected
/// Initializing with a negative signed integer creates an index relative to the last element.
/// Index -1 refers to the last element.
///
struct Index : Union<ssize_t, size_t>
{
    using Union<ssize_t, size_t>::Union;

    ///
    /// Returns an unsigned index into an array. Does not check if the index is in bounds.
    /// @param a The array to compute the index for.
    ///
    NODISCARD FORCEINLINE constexpr auto computeUnchecked(auto const& a) const
    {
        size_t i_ = match(
            [&](size_t k) { return k; },
            [&](ssize_t k) {  // Possible negative index
                return k < 0 ? size_t(ssize_t(a.length()) + k) : size_t(k);
            });
        return i_;
    }

    ///
    /// Returns an unsigned index into an array. Checks if the index is in bounds.
    /// @param a The array to compute the index for.
    ///
    NODISCARD FORCEINLINE constexpr auto compute(auto const& a) const
    {
        size_t i = computeUnchecked(a);
        Assert(i < a.length(), ASMS_BOUNDS);
        return i;
    }

    ///
    /// Asserts that the index is not negative, and if so, returns the index as an unsigned integer.
    ///
    NODISCARD FORCEINLINE constexpr usize assertPositive() const
    {
        size_t i = match(
            [&](size_t k) { return k; },
            [&](ssize_t k) {
                Assert(k >= 0, ASMS_PARAMETER);
                return size_t(k);
            });
        return i;
    }
};


///
/// A non-owning reference to an array
///
template<typename T>
struct ArrayRef
{
    T* _ptr;
    size_t _length;

    ///
    /// Constructor from pointer and length
    ///
    FORCEINLINE explicit constexpr ArrayRef(T* ptr_, size_t length_) noexcept
        : _ptr(ptr_), _length(length_)
    {}
    ///
    /// Constructor from initializer list
    ///
    FORCEINLINE constexpr ArrayRef(std::initializer_list<T> const& v) noexcept
        : _ptr(const_cast<T*>(v.begin())), _length(v.size())
    {}
    ///
    /// Constructor from character literal
    ///
    template<unsigned N>
    FORCEINLINE constexpr ArrayRef(T const (&literal)[N]) noexcept
        : _ptr(const_cast<T*>(literal)), _length(N)
    {}
    ///
    /// Index operator. Performs bounds checking.
    ///
    NODISCARD FORCEINLINE constexpr T const& operator[](Index const& i) const
    {
        size_t i_ = i.compute(*this);
        Assert(i_ < length(), ASMS_BOUNDS);
        UNSAFE({ return _ptr[i_]; });
    }
    ///
    /// Index operator that does not perform bounds checking.
    ///
    NODISCARD FORCEINLINE constexpr T const& operator()(Index const& i) const
    {
        size_t i_ = i.computeUnchecked(*this);
        UNSAFE({ return _ptr[i_]; });
    }
    ///
    /// Returns how many elements are in the array.
    ///
    NODISCARD FORCEINLINE constexpr auto length() const noexcept { return _length; }
    ///
    /// Returns size in bytes of all elements in the array.
    ///
    NODISCARD FORCEINLINE constexpr auto sizeBytes() const noexcept { return _length * sizeof(T); }

    /// First element.
    NODISCARD FORCEINLINE constexpr T const* begin() const noexcept { return _ptr; }
    /// One past the last element.
    NODISCARD FORCEINLINE constexpr T const* end() const noexcept { return UNSAFE(_ptr + _length); }

    /// Returns a pointer to the array data
    NODISCARD FORCEINLINE constexpr T const* data() const noexcept { return _ptr; }
    /// Returns a pointer to the array data
    NODISCARD FORCEINLINE constexpr T* data() noexcept { return _ptr; }

    ///
    /// Returns the smallest element in the array.
    ///
    constexpr T const& min(this auto&& self)
    {
        T m = self(0);
        for (usize i = 1; i < self.length(); i++) {
            m = ::cm::min(m, self(i));
        }
        return m;
    }

    ///
    /// Returns the largest element in the array.
    ///
    constexpr T const& max(this auto&& self)
    {
        T m = self(0);
        for (usize i = 1; i < self.length(); i++) {
            m = ::cm::max(m, self(i));
        }
        return m;
    }

    constexpr static void outputString(ArrayRef<T> const& a, auto const& out);
};

template<typename T>
ArrayRef(T*, size_t) -> ArrayRef<T>;

template<typename T>
ArrayRef(std::initializer_list<T> const&) -> ArrayRef<T>;

template<typename T, unsigned N>
ArrayRef(T const (&literal)[N]) -> ArrayRef<T>;


using StringRef = ArrayRef<char>;

}  // namespace cm
#endif
