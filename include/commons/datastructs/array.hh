/*
   Copyright 2024 Anthony A. Constantinescu.

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
#include "commons/datastructs/array_iterator.hh"
#include <commons/datastructs/collection.hh>


namespace cm {

UNSAFE_BEGIN

constexpr inline u32 ARRAY_LENGTH_UNSPECIFIED = 0xffffffff;


template<typename T, u32 L = ARRAY_LENGTH_UNSPECIFIED>
class Array;

namespace CollectionType {
template<typename T, u32 L = ARRAY_LENGTH_UNSPECIFIED>
class Array {
public:
    using ItsElementType = T;

    template<typename SomeType = T>
    using ItsType = ::cm::Array<SomeType, L>;
};
}

template<typename T, u32 L>
struct ArrayBase
{
    T _data[L];
};

template<typename T>
struct ArrayBase<T, ARRAY_LENGTH_UNSPECIFIED>
{
    T* _data = nullptr;
    usize _length = 0;
};

///
/// An Array with a size set at compile time.
/// @tparam T Array element type
/// @tparam L Array length
///
template<typename T, u32 L>
class Array : protected ArrayBase<T, L>,
              public LinearIteratorComponent<Array<T, L>, T>,
              public Collection<CollectionType::Array<T, L>> {
protected:
    using Base = ArrayBase<T, L>;

public:
    using ElementType = T;

    ///
    /// Default constructor
    ///
    Array() = default;

    ///
    /// Destructor
    ///
    ~Array()
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            if (Base::_data) {
                delete[] Base::_data;
            }
        }
    }

    ///
    /// Initialize an array with a predetermined variable length
    ///
    Array(Index const& len) requires ((L == ARRAY_LENGTH_UNSPECIFIED))
    {
        usize len_ = len.assertPositive();
        Base::_data = new T[len_]{};
        Base::_length = len_;
    }

    ///
    /// Constructor from pointer and length
    ///
    Array(T* ptr, usize len)
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            Base::_data = new T[len];
            Base::_length = len;
            for (usize i = 0; i < len; i++)
                Base::_data[i] = ptr[i];

        } else {
            for (usize i = 0; i < min(L, len); i++)
                Base::_data[i] = ptr[i];
        }
    }

    ///
    /// Constructors from initializer list
    ///
    Array(std::initializer_list<T> const& v)
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            Base::_data = new T[v.size()];
            Base::_length = v.size();
        }
        for (usize i = 0; i < v.size(); i++)
            Base::_data[i] = v.begin()[i];
    }

    ///
    /// Constructor from character literal
    ///
    template<unsigned N>
    Array(T const (&values)[N])
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            Base::_data = new T[N];
            Base::_length = N;
        } else {
            static_assert(N <= L, "Too large");
        }
        for (unsigned int i = 0; i < N; i++)
            Base::_data[i] = values[i];
    }

    ///
    /// Copy constructor
    ///
    Array(Array<T, L> const& other)
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            Base::_data = new T[other.length()];
            Base::_length = other.length();
        }
        for (usize i = 0; i < other.length(); i++) {
            Base::_data[i] = other._data[i];
        }
    }

    ///
    /// Move constructor
    ///
    constexpr Array(Array<T, L>&& other) noexcept
    {
        Base::_data = other._data;
        Base::_length = other._length;
        other._data = nullptr;
        other._length = 0;
    }

    ///
    /// Copy assignment operator
    ///
    constexpr Array& operator=(Array<T, L> const& other)
    {
        this->~Array<T, L>();
        new (this) Array<T, L>(other);
        return *this;
    }

    ///
    /// Move assignment operator
    ///
    constexpr Array& operator=(Array<T, L>&& other)
    {
        this->~Array<T, L>();
        new (this) Array<T, L>(other);
        return *this;
    }

    ///
    /// Returns a new array which contains this array's elements repeated n times.
    /// @param count the number of times to repeat
    ///
    auto times(Index const& count)
    {
        usize n = count.assertPositive();
        auto newArray = Array<T>(this->length() * n);

        for (usize i = 0; i < newArray.length(); i += this->length()) {
            for (usize j = 0; j < this->length(); j++) {
                newArray[i + j] = (*this)[j];
            }
        }
        return newArray;
    }

    ///
    /// Bounds-checking index operator.
    ///
    constexpr auto& operator[](this auto&& self, Index i) UNSAFE({ return self._data[i.compute(self)]; });

    ///
    /// Returns a pointer to the underlying array's buffer
    ///
    constexpr inline auto data(this auto&& self) { return &self._data[0]; }

    ///
    /// Returns the number of elements in the array.
    ///
    constexpr inline auto length() const
    {
        if constexpr (L != ARRAY_LENGTH_UNSPECIFIED) {
            return L;
        } else {
            return Base::_length;
        }
    }

    ///
    /// Returns the total size bytes of the elements in the array.
    ///
    constexpr inline auto sizeBytes() const { return length() * sizeof(T); }

    ///
    /// Calls a function on each element of the array.
    /// @param visitor A function to call on each element
    ///
    constexpr void forEach(this auto&& self, auto visitor)
    {
        for (auto& e : self) {
            visitor(e);
        }
    }

    ///
    /// Generates a string representation for the Array.
    /// @param array The array
    /// @param out A function that sequentially receives each character in the string representation of array.
    ///
    constexpr static auto outputString(Array<T, L> const& array, auto const& out)
    {
        ArrayRef<T>(array).outputString(array, out);
    }

    ///
    /// For each index I in indices, return the array such that array[I] = setter(I).
    /// @param indices An iterable set of indices
    /// @param setter The setter (may take an optional third argument referencing the existing array)
    ///
    template<typename Self>
    constexpr auto set(this Self&& self, auto const& indices, auto const& setter)
    {
        auto const actualSetter = [&](auto const& index) {
            if constexpr (IsCallableWith<decltype(setter), decltype(index), Self>) {
                return setter(index, self);
            } else {
                return setter(index);
            }
        };
        if constexpr (IsMutable<Self>) {
            if constexpr (IsIterable<decltype(indices)>) {
                for (usize index : indices) {
                    self[index] = actualSetter(index);
                }
            } else {
                self[index] = actualSetter(index);
            }
            return self;
        } else {
            return Array(self).set(indices, setter);
        }
    }

    ///
    /// For each row R in rows and for each column C in columns,
    /// Return the array such that array[R][C] = setter(R, C)
    /// @param rows An iterable set of rows
    /// @param columns An iterable set of columns
    /// @param setter The setter (may take an optional third argument referencing the existing array)
    ///
    template<typename Self>
    constexpr auto set(this Self&& self, auto const& rows, auto const& columns, auto const& setter)
    {
        auto const actualSetter = [&](auto const& row, auto const& col) {
            if constexpr (IsCallableWith<decltype(setter), decltype(row), decltype(col), Self>) {
                return setter(row, col, self);
            } else {
                return setter(row, col);
            }
        };
        if constexpr (IsMutable<Self>) {
            if constexpr (IsIterable<decltype(rows)> && IsIterable<decltype(columns)>) {
                for (auto const& row : rows) {
                    for (auto const& col : columns) {
                        self[row][col] = actualSetter(row, col);
                    }
                }
            } else if constexpr (IsIterable<decltype(rows)> && !IsIterable<decltype(columns)>) {
                for (auto row : rows) {
                    self[row][columns] = actualSetter(row, columns);
                }
            } else if constexpr (!IsIterable<decltype(rows)> && IsIterable<decltype(columns)>) {
                for (auto col : columns) {
                    self[rows][col] = actualSetter(rows, col);
                }
            } else {
                self[rows][columns] = actualSetter(rows, columns);
            }
            return self;
        } else {
            return Array(self).set(rows, columns, setter);
        }
    }

    ///
    /// An alias for operator[]
    ///
    constexpr auto element(this auto&& self, usize row) { return self[row]; }

    ///
    /// An alias for operator[] for a 2D array
    ///
    constexpr auto element(this auto&& self, usize row, usize col) { return self[row][col]; }

    ///
    /// Index operator that does not perform bounds checking.
    ///
    constexpr auto& operator()(Index i) UNSAFE({ return Base::_data[i.computeUnchecked(*this)]; });
    constexpr auto const& operator()(Index i) const { UNSAFE(return Base::_data[i.computeUnchecked(*this)];) }

    ///
    /// 2D Indexing operator that does not perform bounds checking.
    ///
    constexpr auto const& operator()(Index row, auto const& col) const
    {
        UNSAFE(return Base::_data[row.computeUnchecked(*this)][col];)
    }
};


template<typename T>
Array(std::initializer_list<T> const&) -> Array<T, ARRAY_LENGTH_UNSPECIFIED>;


///
/// Utility function for initializing a 2D Array (i.e. an array of arrays). Note that this is nesting arrays inside of
/// an array, so it is not a contiguous block of memory.
/// It's more like a "jagged array" https://en.wikipedia.org/wiki/Irregular_matrix
///
/// For a 2D array where each element resides in a contiguous block of memory, choose Matrix2D instead.
/// @param rows How many rows
/// @param cols How many cols
/// @return An empty array of arrays
///
template<typename T>
constexpr auto Array2D(auto rows, auto cols)
{
    return Array({Array<T>(cols)}).times(rows);
}

}  // namespace cm

UNSAFE_END
