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

/// The base class for all array-type collections. Defines all read-only methods for array types.
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

    /// Returns how many elements are in the array.
    virtual constexpr usize length() const = 0;

    /// Returns size in bytes of all elements in the array.
    virtual constexpr usize sizeBytes() const { return length() * sizeof(T); }

    /// Returns a pointer to the array data.
    virtual constexpr T const* data() const = 0;

    /// Bounds-checking index operator, returns a const reference
    virtual constexpr T const& operator[](Index const& i) const = 0;

    /// Index operator that is not required to perform bounds checking.
    virtual constexpr T const& operator()(Index const& i) const { return (*this)[i]; }

    /// An alias for operator[]
    constexpr auto element(usize row) { return (*this)[row]; }

    /// An alias for operator[] for a 2D array
    constexpr auto element(usize row, usize col) { return (*this)[row][col]; }

    /// 2D Indexing operator that does not perform bounds checking.
    constexpr auto const& operator()(Index row, auto const& col) const [[clang::lifetimebound]]
    {
        UNSAFE(return this->operator()(row)[col];)
    }

    /// Performs a deep equality comparison of two arrays.
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

    /// Performs a deep comparison of two arrays.
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
                return Iterable<IArray>::compare(other);
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

/// A non-owning reference to an array
template<typename T>
struct ArrayRef : IArray<T>
{
private:
    T const* _ptr = nullptr;
    size_t _length = 0;

public:
    constexpr ArrayRef() noexcept = default;

    /// Constructor from pointer and length
    /// @param ptr_ The pointer
    /// @param length_ The length
    explicit constexpr ArrayRef(T const* ptr_, size_t length_) noexcept
        : _ptr(ptr_), _length(length_)
    {}

    /// Constructor from initializer list. This constructs an ArrayRef with reference to TEMPORARY data. As such, it is
    /// only safe for doing something like the following:
    /// \code{.cpp}
    /// auto m = ArrayRef({1, 2, 3, 4, 5}).mean();
    /// \endcode
    /// The following is unsafe, however (storing a permanent reference to a temporary)
    /// \code{.cpp}
    /// auto x = ArrayRef({1, 2, 3, 4, 5});
    /// \endcode
    /// Use of the above will issue a warning.
    constexpr ArrayRef(::std::initializer_list<T> const& v) noexcept
        : _ptr(const_cast<T*>(v.begin())), _length(v.size())
    {}

    ///
    /// Constructor from character literal
    ///
    template<unsigned N>
    constexpr ArrayRef(T const (&literal)[N]) noexcept
        : _ptr(const_cast<T*>(literal)), _length(N)
    {}

    //[[clang::callable_when(unconsumed)]]
    constexpr ArrayRef(ArrayRef const&) noexcept = default;

    //[[clang::callable_when(unconsumed)]]
    constexpr ArrayRef(ArrayRef&&) noexcept = default;

    // Explicitly defaulted move assignment operator (C++11 and later)
    //[[clang::callable_when(unconsumed)]]
    constexpr ArrayRef& operator=(ArrayRef&&) noexcept = default;

    //[[clang::callable_when(unconsumed)]]
    constexpr ArrayRef& operator=(ArrayRef const&) noexcept = default;

    /// Index operator. Performs bounds checking.
    /// @param i the index
    constexpr T const& operator[](Index const& i) const override
    {
        auto const i_ = i.compute(*this);
        Assert(i_ < length(), ASSERT_MSG_BOUNDS);
        UNSAFE({ return _ptr[i_]; });
    }

    /// Index operator that does not perform bounds checking.
    constexpr T const& operator()(Index const& i) const override
    {
        auto const i_ = i.computeUnchecked(*this);
        UNSAFE({ return _ptr[i_]; });
    }

    constexpr usize length() const override { return _length; }
    constexpr T const* data() const override { return _ptr; }
    constexpr T* data() { return const_cast<T*>(_ptr); }
};

// Deduction guides for ArrayRef

template<typename T>
ArrayRef(T*, size_t) -> ArrayRef<T>;
template<typename T>
ArrayRef(::std::initializer_list<T>&&) -> ArrayRef<T>;
template<typename T, unsigned N>
ArrayRef(T const (&literal)[N]) -> ArrayRef<T>;

/// An owning fixed size array
template<typename T, usize Length>
struct FixedArray : IArray<T>
{
private:
    T _buf[Length]{};

public:
    constexpr FixedArray() noexcept = default;


    explicit constexpr FixedArray(T* ptr, usize const len)
    {
        Assert(ptr, ASMS_INVALID(ptr));
        for (usize i = 0; i < min(len, Length); i++)
            _buf[i] = ptr[i];
    }

    template<unsigned N>
    constexpr FixedArray(T const (&values)[N])  // NOLINT
    {
        for (usize i = 0; i < min(N, Length); i++)
            _buf[i] = values[i];
    }

    /// Constructors from initializer list
    constexpr FixedArray(std::initializer_list<T> const& l)
    {
        for (usize i = 0; i < min(l.size(), Length); i++)
            _buf[i] = l[i];
    }

    /// Index operator. Performs bounds checking.
    /// @param i the index
    constexpr T const& operator[](Index const& i) const override
    {
        auto const i_ = i.compute(*this);
        Assert(i_ < length(), ASSERT_MSG_BOUNDS);
        UNSAFE({ return _buf[i_]; });
    }

    /// Index operator that does not perform bounds checking.
    constexpr T const& operator()(Index const& i) const override
    {
        auto const i_ = i.computeUnchecked(*this);
        UNSAFE({ return _buf[i_]; });
    }

    /// Index operator. Performs bounds checking.
    /// @param i the index
    constexpr T& operator[](Index const& i)
    {
        auto const i_ = i.compute(*this);
        Assert(i_ < length(), ASSERT_MSG_BOUNDS);
        UNSAFE({ return _buf[i_]; });
    }

    /// Index operator that does not perform bounds checking.
    constexpr T& operator()(Index const& i)
    {
        auto const i_ = i.computeUnchecked(*this);
        UNSAFE({ return _buf[i_]; });
    }

    constexpr usize length() const override { return Length; }
    constexpr T const* data() const override { return _buf; }
    constexpr T* data() { return _buf; }
};

/// An owning reference to an array
template<typename T>
struct Array : IArray<T>
{
private:
    T* _buf = nullptr;
    size_t _length = 0;

public:
    constexpr Array() noexcept = default;


    explicit Array(usize const len)
    {
        _buf = new T[len]{};
        _length = len;
    }

    explicit Array(T* ptr, usize const len)
    {
        _buf = new T[len];
        _length = len;
        for (usize i = 0; i < len; i++)
            _buf[i] = ptr[i];
    }

    template<unsigned N>
    Array(T const (&values)[N])  // NOLINT
    {
        _buf = new T[N]{};
        _length = N;
        for (usize i = 0; i < N; i++)
            _buf[i] = values[i];
    }

    /// Constructors from initializer list
    Array(std::initializer_list<T> const& l)
    {
        _buf = new T[l.size()];
        _length = l.size();
        for (usize i = 0; i < l.size(); i++)
            _buf[i] = l[i];
    }

    /// Copy constructor
    Array(Array const& other)
    {
        _buf = new T[other._length]{};
        _length = other._length;
        for (usize i = 0; i < other._length; i++)
            _buf[i] = other._buf[i];
    }

    /// Move constructor
    Array(Array&& other) noexcept
    {
        _buf = other._buf;
        _length = other._length;
        other._buf = nullptr;
        other._length = 0;
    }
    /// Copy assignment operator
    Array& operator=(Array const& other)
    {
        if (this != &other) {
            delete[] _buf;
            _buf = other._buf;
            _length = other._length;
            for (usize i = 0; i < other._length; i++)
                _buf[i] = other._buf[i];
        }
        return *this;
    }

    /// Move assignment operator
    Array& operator=(Array&& other) noexcept
    {
        if (this != &other) {
            delete[] _buf;
            _buf = other._buf;
            _length = other._length;
            other._buf = nullptr;
            other._length = 0;
        }
        return *this;
    }

    /// Destructor
    ~Array() override
    {
        if (_buf != nullptr) {
            delete[] _buf;
        }
    }

    /// Index operator. Performs bounds checking.
    /// @param i the index
    constexpr T const& operator[](Index const& i) const override
    {
        auto const i_ = i.compute(*this);
        Assert(i_ < length(), ASSERT_MSG_BOUNDS);
        UNSAFE({ return _buf[i_]; });
    }

    /// Index operator that does not perform bounds checking.
    constexpr T const& operator()(Index const& i) const override
    {
        auto const i_ = i.computeUnchecked(*this);
        UNSAFE({ return _buf[i_]; });
    }

    /// Index operator. Performs bounds checking.
    /// @param i the index
    constexpr T& operator[](Index const& i)
    {
        auto const i_ = i.compute(*this);
        Assert(i_ < length(), ASSERT_MSG_BOUNDS);
        UNSAFE({ return _buf[i_]; });
    }

    /// Index operator that does not perform bounds checking.
    constexpr T& operator()(Index const& i)
    {
        auto const i_ = i.computeUnchecked(*this);
        UNSAFE({ return _buf[i_]; });
    }

    constexpr usize length() const override { return _length; }
    constexpr T const* data() const override { return _buf; }
    constexpr T* data() { return _buf; }

    /// Returns a new array which contains this array's elements repeated n times.
    /// @param count the number of times to repeat
    Array times(Index const& count)
    {
        usize n = count.assertPositive();
        auto newArray = Array(this->length() * n);

        for (usize i = 0; i < newArray.length(); i += this->length()) {
            for (usize j = 0; j < this->length(); j++) {
                newArray[i + j] = (*this)[j];
            }
        }
        return newArray;
    }

    /// For each index I in indices, return the array such that array[I] = setter(I).
    /// @param indices An iterable set of indices
    /// @param setter The setter (may take an optional third argument referencing the existing array)
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
                self[indices] = actualSetter(indices);
            }
            return self;
        } else {
            return Array(self).set(indices, setter);
        }
    }

    /// For each row R in rows and for each column C in columns,
    /// Return the array such that array[R][C] = setter(R, C)
    /// @param rows An iterable set of rows
    /// @param columns An iterable set of columns
    /// @param setter The setter (may take an optional third argument referencing the existing array)
    template<typename Self>
    constexpr auto set(this Self&& self, auto const& rows, auto const& columns, auto const& setter)
    {
        auto const actualSetter = [&]<typename T0, typename T1>(T0 const& row, T1 const& col) {
            if constexpr (IsCallableWith<decltype(setter), T0, T1, Self>) {
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
};

/// Utility function for initializing a 2D Array (i.e. an array of arrays). Note that this is nesting arrays inside of
/// an array, so it is not a contiguous block of memory.
/// It's more like a "jagged array" https://en.wikipedia.org/wiki/Irregular_matrix
///
/// For a 2D array where each element resides in a contiguous block of memory, choose Matrix2D instead.
/// @param rows How many rows
/// @param cols How many cols
/// @return An empty array of arrays
template<typename T>
constexpr auto Array2D(auto rows, auto cols)
{
    return Array({Array<T>(cols)}).times(rows);
}

}  // namespace cm
#endif
