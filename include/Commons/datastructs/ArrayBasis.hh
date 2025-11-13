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
#include <Commons/core.hh>
#include "ArrayIterator.hh"
#include <Commons/datastructs/ArrayUtil.hh>
#include "Collection.hh"


namespace cm {

class String;

constexpr inline u32 ARRAY_LENGTH_UNSPECIFIED = 0xffffffff;
template<typename T, u32 Length = ARRAY_LENGTH_UNSPECIFIED>
class Array;

namespace CollectionType {
template<typename T, u32 Length>
class Array {
public:
    using ItsElementType = T;

    template<typename SomeType = T>
    using ItsType = ::cm::Array<SomeType, Length>;
};
}

/**
 * @brief Functions shared between fixed arrays and resizable arrays.
 */
template<typename T, u32 Length>
class ArrayBasis : public BufferIterable<ArrayBasis<T, Length>, T>,
                   public Collection<CollectionType::Array<T, Length>> {
public:
    using CVRefRemovedType = CVRefRemoved<T>;
    using PointerRemovedType = PointerRemoved<CVRefRemovedType>;

    constexpr inline auto& getDerived() { return *static_cast<Array<T, Length>*>(this); }
    constexpr inline auto const& getDerived() const { return *static_cast<Array<T, Length> const*>(this); }

    /**
     * If `T` is a pointer, reference, or value of a class type, is a type alias to that underlying class type.
     * Otherwise is a dummy type (Without the dummy type, certain functions such as those accepting a member function
     * callback would stop the class from compiling)
     */
    using UnderlyingClassType = CVRefRemoved<decltype(*([]() {
        if constexpr (IsClass<RefRemoved<PointerRemoved<T>>>) {
            return reinterpret_cast<PointerRemoved<CVRefRemoved<T>>*>(0xdeadbeef);
        } else {
            return reinterpret_cast<Nothing*>(0xdeadbeef);
        }
    }()))>;

    /**
     * @brief Gets element at index.
     * @param index The index in [0, length() - 1]
     * @return A reference to the element.
     */
    constexpr inline T& operator[](u32 index)
    {
        Assert(0 <= index && index < getDerived().length());
        UNSAFE({ return getDerived().data()[index]; })
    }

    /**
     * @brief Gets element at index.
     * @param index The index in [0, length() - 1]
     * @return A reference to the element.
     */
    constexpr inline T const& operator[](u32 index) const
    {
        Assert(0 <= index && index < getDerived().length());
        UNSAFE({ return getDerived().data()[index]; })
    }

    constexpr void forEach(auto visitor) const
    {
        for (auto const& e : *this) {
            visitor(e);
        }
    }

    // /**
    //  * @brief Joins two arrays. The first half of the resulting array will contain the left side array, the second
    //  * half will contain the elements of the right side array.
    //  *  The array to append.
    //  * @return The result of this array combined with another array.
    //  */
    // Array<T, Length> operator+(Array<T, Length> const& refRhs) requires (Length == ARRAY_LENGTH_UNSPECIFIED)
    // {
    //     Array<T> result{};
    //     result.reserve(refRhs.length());
    //     copy(result, 0, *this, 0, length());
    //     copy(result, length(), refRhs, 0, refRhs.length());
    //     return result;
    // }


    constexpr inline auto length() const { return getDerived().length(); }
    constexpr inline bool empty() const { return getDerived().length() == 0; }
    constexpr inline auto* iterable_begin() { return getDerived().data(); }
    constexpr inline auto const* iterable_begin() const { return getDerived().data(); }
    constexpr inline auto* iterable_end() UNSAFE({ return &getDerived().data()[getDerived().length()]; });
    constexpr inline auto const* iterable_end() const UNSAFE({ return &getDerived().data()[getDerived().length()]; })
};
}  // namespace cm
