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
#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else

namespace cm {

///
/// The base class for all array-type collections. Defines all read-only methods for array types.
///
template<typename T>
struct IArray : Iterable<IArray<T>>,
                LinearIteratorComponent<IArray<T>, T const>,
                IEquatable<IArray<T>>,
                IComparable<IArray<T>>
{
    constexpr IArray() = default;
    constexpr IArray(IArray const&) = default;
    constexpr IArray& operator=(IArray const&) = default;

    virtual constexpr ~IArray() = default;

    ///
    /// Returns how many elements are in the array.
    ///
    virtual constexpr usize length() const = 0;

    ///
    /// Returns size in bytes of all elements in the array.
    ///
    virtual constexpr usize sizeBytes() const { return length() * sizeof(T); }

    ///
    /// Returns a pointer to the array data.
    ///
    virtual constexpr T const* data() const = 0;

    ///
    /// Bounds-checking index operator.
    ///
    virtual constexpr T const& operator[](Index const& i) const = 0;

    ///
    /// Index operator that is not required to perform bounds checking.
    ///
    virtual constexpr T const& operator()(Index const& i) const { return (*this)[i]; }

    ///
    /// Performs a deep equality comparison of two arrays.
    ///
    constexpr bool equals(IArray const& other)
    {
        if consteval {
            return Iterable<IArray>::equals(other);
        } else {
            if (this == &other) {
                return true;
            }
            // At runtime, select the C library memcmp if possible, which is highly optimized.
            if constexpr (IsPrimitiveData<T>) {
                if (length() != other.length()) {
                    return false;
                }
                UNSAFE_BEGIN;
                return __builtin_memcmp(data(), other.data(), sizeBytes()) == 0;
                UNSAFE_END;
            } else {
                return Iterable<IArray>::equals(other);
            }
        }
    }

    ///
    /// Performs a deep comparison of two arrays.
    ///
    constexpr int compare(IArray const& other)
    {
        if consteval {
            return Iterable<IArray>::compare(other);
        } else {
            // At runtime, select the C library memcmp if possible, which is highly optimized.
            if constexpr (IsPrimitiveData<T>) {
                if (this == &other) {
                    return 0;
                }
                UNSAFE_BEGIN;
                int res = __builtin_memcmp(data(), other.data(), min(sizeBytes(), other.sizeBytes()));
                UNSAFE_END;
                if (res != 0) {
                    return res;
                }
                // If they are identical up to min_len, the shorter array is "less than"
                if (length() < other.length()) {
                    return -1;
                }
                if (length() > other.length()) {
                    return 1;
                }
                return 0;
            } else {
                return Iterable<IArray>::compare(other);
            }
        }
    }
};


}  // namespace cm

#endif
