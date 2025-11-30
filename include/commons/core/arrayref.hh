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


namespace cm {


///
/// A non-owning reference to an array
///
template<typename T>
struct ArrayRef : public LinearIteratorComponent<ArrayRef<T>, T const>,
                  public IEquatable<ArrayRef<T>>,
                  public IComparable<ArrayRef<T>>
{
    T const* _ptr;
    size_t _length;

    ///
    /// Constructor from pointer and length
    ///
    FORCEINLINE explicit constexpr ArrayRef(T const* ptr_, size_t length_) noexcept
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

    /// Returns a pointer to the array data
    NODISCARD FORCEINLINE constexpr T const* data() const noexcept { return _ptr; }
    /// Returns a pointer to the array data
    /// NODISCARD FORCEINLINE constexpr T* data() noexcept { return _ptr; }

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

    ///
    /// Generates a string representation of an ArrayRef. Outputs (within square brackets) the string representation of
    /// each element in the array, delimited by commas.
    ///
    constexpr static void outputString(ArrayRef<T> const& self, auto const& out)
    {
        out('[');
        if (self.length() != 0) {
            usize i = 0;
            for (; i < self.length() - 1; i++) {
                auto const& value = self(i);
                OutputString(value, out);
                out(',');
                out(' ');
            }
            for (; i < self.length(); i++) {
                auto const& value = self(i);
                OutputString(value, out);
            }
        }
        out(']');
    }

    ///
    /// Performs a deep equality comparison of two arrays.
    ///
    constexpr bool equals(this ArrayRef<T> const& self, ArrayRef<T> const& other)
    {
        if (&self == &other) {
            return true;
        }
        if (self.length() != other.length()) {
            return false;
        }
        // Generic comparison loop
        auto cmp = [&]() constexpr {
            for (auto i : Range(self.length())) {
                if (self(i) != other(i)) {
                    return false;
                }
            }
            return true;
        };
        if consteval {
            // At compile time, use the generic version
            return cmp();
        } else {
            // At runtime, select the C library memcmp if possible, which is highly optimized.
            if constexpr (IsPrimitiveData<T>) {
                UNSAFE_BEGIN;
                return __builtin_memcmp(self.data(), other.data(), self.sizeBytes()) == 0;
                UNSAFE_END;
            } else {
                return cmp();
            }
        }
    }

    ///
    /// Performs a deep equality comparison of two arrays that **attempts** to mitigate timing vulnerabilities.
    /// Generally slower than equals(), but ensures that comparison always takes the same amount of time for a given
    /// array length.
    ///
    constexpr bool equalsTimesafe(this ArrayRef<T> const& self, ArrayRef<T> const& other)
    {
        // No early return as that is a timing vulnerability.
        i64 remaining = i64(other.length());
        i64 i = 0;

        int notEqual = 0;
        while (i < i64(self.length())) {
            notEqual |= (self(i) != other(i));
            ++i;
            --remaining;
        }
        return !notEqual && (remaining == 0);
    }

    ///
    /// Performs a deep comparison of two arrays.
    ///
    constexpr int compare(this ArrayRef<T> const& self, ArrayRef<T> const& other)
    {
        if (&self == &other) {
            return 0;
        }
        if (self.length() != other.length()) {
            return Compare(self.length(), other.length());
        }
        // Generic comparison loop
        auto cmp = [&]() constexpr {
            for (auto i : Range(self.length())) {
                if (self(i) < other(i)) {
                    return -1;
                } else if (self(i) > other(i)) {
                    return 1;
                }
            }
            return 0;
        };
        if consteval {
            // At compile time, use the generic version
            return cmp();
        } else {
            // At runtime, select the C library memcmp if possible, which is highly optimized.
            if constexpr (IsPrimitiveData<T>) {
                UNSAFE_BEGIN;
                return __builtin_memcmp(self.data(), other.data(), self.sizeBytes());
                UNSAFE_END;
            } else {
                return cmp();
            }
        }
    }

    ///
    /// Performs a deep comparison of two arrays that **attempts** to mitigate timing vulnerabilities.
    /// Generally slower than compare(), but ensures that comparison always takes the same amount of time for a given
    /// array length.
    ///
    constexpr int compareTimesafe(this ArrayRef<T> const& self, ArrayRef<T> const& other)
    {
        // No early return as that is a timing vulnerability.
        i64 remaining = i64(other.length());
        i64 i = 0;
        int notEqual = 0;
        int sign = 0;

        while (i < i64(self.length())) {
            int compared = Compare(self(i), other(i));
            notEqual |= (compared != 0);
            // Once sign is set, it cannot be changed.
            sign |= (compared * (sign == 0));
            ++i;
            --remaining;
        }
        return (notEqual * sign) - !!remaining;
    }

    /*


    int memcmp_timesafe(void const* self, void const* other, size_t len)
    {
        size_t i = 0;
        int notEqual = 0;
        int sign = 0;

        while (i < len) {
            int comparison = Compare(self(i), other(i));
            notEqual |= (comparison != 0);
            // Once sign is set, it cannot be changed.
            sign |= (comparison * (sign == 0));
            ++i;
        }
        return !notEqual * sign;
    }


    */
};


///
/// Deduction guides for ArrayRef
///

template<typename T>
ArrayRef(T*, size_t) -> ArrayRef<T>;

template<typename T>
ArrayRef(std::initializer_list<T>&&) -> ArrayRef<T>;

template<typename T, unsigned N>
ArrayRef(T const (&literal)[N]) -> ArrayRef<T>;


}  // namespace cm

#endif
