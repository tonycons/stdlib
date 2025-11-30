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

template<typename Type>
struct SparseArray;

/**
 * @brief Implements type erasure and list manipulation for SparseArray.
 */
namespace impl {
class SparseArray {
protected:
    template<typename T>
    friend struct ::cm::SparseArray;


    struct Container
    {
        void _clear();
        void* _get(void const* pIndex) const;

        void** _data;
        ClassRef _objclass;
        usize _length;
    };
};
}  // namespace impl


struct SparseArrayBase
{};


///
/// An array that is optimized for having most of its elements zero or empty.
/// In other words, it functions as an extremely large array where an element at a given index are most likely zero or
/// empty. Its index bounds are [0, 2^N - 1] where N is the number of bits in the index.
///
/// This implementation uses a binary tree where each leaf in the tree is a slice of bits from the index value.
///
UNSAFE_BEGIN
template<typename Type>
struct SparseArray
{
    Type** data;
    usize _length;
    // firstIndex, lastIndex

public:
    SparseArray() { this->data = new Type*[256]{}; }

    ~SparseArray() { clear(); }

    void clear()
    {
        auto ptr = this->data;

        auto _ = [&](this auto const& next_, unsigned depth) -> void {
            if (depth == 0) {
                for (int k = 0; k < 256; k++) {
                    if (ptr[k]) {
                        delete ptr;
                    }
                }
            } else {
                for (int k = 0; k < 256; k++) {
                    if (ptr[k] != nullptr) {
                        ptr = reinterpret_cast<Type**>(ptr[k]);
                        next_(depth - 1);
                        delete[] ptr;
                    }
                }
            }
        };
        _(8);
    }


    void set(u64 index, Type const& value) noexcept
    {
        auto ptr = this->data;
        //_Pragma("clang loop unroll(disable)")  //
        for (int _ = 0; _ < 7; _++, index >>= 8) {
            auto i = static_cast<u8>(index);
            ptr[i] = !ptr[i] ? reinterpret_cast<Type*>(new Type*[256]{}) : ptr[i];
            ptr = reinterpret_cast<Type**>(ptr[i]);
        }
        auto i = static_cast<u8>(index);
        if (!ptr[i]) {
            ptr[i] = new Type();
            _length++;
        }
        *ptr[i] = value;
    }

    Optional<Type> get(u64 index) const noexcept
    {
        auto ptr = this->data;

        for (int _ = 0; _ < 7; _++, index >>= 8) {
            if (ptr == nullptr)
                return None;
            ptr = reinterpret_cast<Type**>(ptr[static_cast<u8>(index)]);
        }
        auto i = static_cast<u8>(index);
        return !ptr[i] ? None : Optional<Type>(*ptr[i]);
    }

    void removeAt(u64 index)
    {
        auto ptr = this->data;
        for (int _ = 0; _ < 7; _++, index >>= 8) {
            if (ptr == nullptr)
                return;
            ptr = reinterpret_cast<Type**>(ptr[static_cast<u8>(index)]);
        }
        auto i = static_cast<u8>(index);
        if (ptr[i]) {
            delete ptr[i];
        }
        if (_length > 0)
            _length--;
    }

    void forEach(auto visitor)
    {
        auto ptr = this->data;

        auto _ = [&](this auto const& next_, unsigned depth) -> void {
            if (depth == 0) {
                for (int k = 0; k < 256; k++) {
                    if (ptr[k])
                        visitor(ptr);
                }
            } else {
                for (int k = 0; k < 256; k++) {
                    if (ptr[k] != nullptr) {
                        ptr = reinterpret_cast<Type**>(ptr[k]);
                        return next_(depth - 1);
                    }
                }
            }
        };
        _(8);
    }

    constexpr static auto INDEX_BITS = 64;
    constexpr static auto GRANULARITY = 256;


    struct Iterator
    {};


    Iterator begin() {}
};


}  // namespace cm
UNSAFE_END
