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
#include <commons/core/iterable.hh>
#include <commons/core/arrayIterator.hh>
#include <initializer_list>

namespace cm {

template<typename T>
struct ArrayRefPointerStorage
{
    usize _length;
    T* _ptr;

    constexpr T* data() { return _ptr; }
    constexpr T const* data() const { return _ptr; }
    constexpr usize length() const { return _length; }
};

///
/// The base class for all array-type collections. Defines all read-only methods for array types.
///
template<typename T, typename S = ArrayRefPointerStorage<T>>
struct ArrayRef : Iterable<ArrayRef<T>>,
                  LinearIteratorComponent<ArrayRef<T>, T const>,
                  IEquatable<ArrayRef<T>>,
                  IComparable<ArrayRef<T>>
{
protected:
    S _storage;

public:
    constexpr ArrayRef() requires (DefaultConstructible<S>)
    = default;
    constexpr ArrayRef(ArrayRef const&) = default;
    constexpr ArrayRef& operator=(ArrayRef const&) = default;
    constexpr ~ArrayRef() = default;


    template<typename... Args>
    explicit constexpr ArrayRef(Args&&... args) requires (Constructible<S, Args...>)
        : _storage{Forward<Args>(args)...}
    {}


    constexpr usize length() const { return _storage.length(); }
    constexpr usize sizeBytes() const { return _storage.length() * sizeof(T); }
    constexpr T const* data() const { return _storage.data(); }
    constexpr T* data() { return _storage.data(); }


    template<typename I>
    constexpr auto& operator[](this auto&& self, I i)
    {
        i = i < 0 ? static_cast<I>(self.length()) + i : i;
        Assert(i < static_cast<I>(self.length()));
        return self.data()[i];
    }

    template<typename I>
    constexpr auto& operator()(this auto&& self, I i)
    {
        i = i < 0 ? static_cast<I>(self.length()) + i : i;
        return self.data()[i];
    }

    /// Performs a deep equality comparison of two arrays.
    constexpr bool equals(ArrayRef const& other) const
    {
        if consteval {
            return Iterable<ArrayRef>::equals(other);
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
                return Iterable<ArrayRef>::equals(other);
            }
        }
    }

    /// Performs a deep comparison of two arrays.
    constexpr int compare(ArrayRef const& other) const
    {
        if consteval {
            return Iterable<ArrayRef>::compare(other);
        } else {
            // At runtime, select the C library memcmp if possible, which is highly optimized.
            if constexpr (IsPrimitiveData<T>) {
                if (this == &other) {
                    return 0;
                }
                UNSAFE_BEGIN;
                int const res = __builtin_memcmp(data(), other.data(), min(sizeBytes(), other.sizeBytes()));
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
                return Iterable<ArrayRef>::compare(other);
            }
        }
    }

    /// Calls a function on each element of the array.
    /// @param visitor A function to call on each element
    constexpr void forEach(this auto&& self, auto visitor)
    {
        for (auto& e : self)
            visitor(e);
    }
};


template<typename T, unsigned N>
struct FixedArrayStorage
{
    T _data[N]{};

    constexpr T* data() { return _data; }
    constexpr T const* data() const { return _data; }
    constexpr usize length() const { return N; }
};


template<typename T, unsigned N>
struct FixedArray : public ArrayRef<T, FixedArrayStorage<T, N>>
{
    using Base = ArrayRef<T, FixedArrayStorage<T, N>>;

    explicit constexpr FixedArray(T const* ptr, usize len)
        : Base(len)
    {
        for (usize i = 0; i < this->length(); i++) {
            (*this)[i] = ptr[i];
        }
    }

    template<unsigned K>
    constexpr FixedArray(T const (&values [[clang::lifetimebound]])[K])
        : Base(K)
    {
        for (usize i = 0; i < this->length(); i++) {
            (*this)[i] = values[i];
        }
    }

    constexpr FixedArray(std::initializer_list<T> const& vs)
    {
        usize i = 0;
        for (auto const& v : vs) {
            (*this)[i++] = v;
        }
    }
};


}  // namespace cm
