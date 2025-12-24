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
/// A structure that functions similarly to a std::vector<unsigned char>.
/// Its purpose is to implement a "growable" piece of contiguous memory.
/// For a typed equivalent (the closest thing to std::vector<T>), see StructVector<T> below.
///
struct ByteVector : IEquatable<ByteVector>
{
private:
    u8* _data;
    usize _length;
    usize _capacity;
    bool _needheap;

    UNSAFE_BEGIN void _ensureDataOnHeap()
    {
        if (!_needheap)
            return;
        Assert(Ptr::isRomData(_data), ASMS_BAD_CIRCUMSTANCE);
        auto old = _data;
        _data = new u8[_capacity];
        memmove(_data, old, _capacity);
        _needheap = false;
        UNSAFE_END;
    }

    UNSAFE_BEGIN void _reallocate()
    {
        Assert(_capacity > 0, ASMS_BUG);
        auto old = _data;
        _data = new u8[size_t(_capacity)];
        memmove(_data, old, _length);
        delete[] old;
        UNSAFE_END;
    }

public:
    constexpr ByteVector() = default;

    ///
    /// Initialize from region of memory
    ///
    inline ByteVector(void const* ptr, usize len) noexcept
    {
        Assert(ptr, ASMS_INVALID(ptr));
        Assert(len, ASMS_INVALID(len));
        if (Ptr::isRomData(ptr)) {
            // This optimization (checking if the pointer has infinite lifetime) means it is unnecessary to make a copy
            // of the data (until it gets modified, then make a copy)
            _data = static_cast<u8*>(const_cast<void*>(ptr));
            _length = _capacity = len;
            _needheap = true;
        } else {
            insert(0, ptr, len);
        }
    }

    ///
    /// Copy constructor
    ///
    inline ByteVector(ByteVector const& other) noexcept
    {
        if (other._data) {
            if (Ptr::isRomData(other._data)) {
                _data = const_cast<u8*>(other._data);
                _length = _capacity = other._length;
                _needheap = true;
            } else {
                insert(0, other._data, other._length);
            }
        }
    }

    ///
    /// Move constructor
    ///
    inline ByteVector(ByteVector&& other) noexcept
    {
        _data = other._data;
        _length = other._length;
        _capacity = other._capacity;
        _needheap = other._needheap;
        other._data = nullptr;
    }

    ///
    /// Copy assignment
    ///
    inline ByteVector& operator=(ByteVector const& other) noexcept
    {
        clear();
        new (this) ByteVector(other);
        return *this;
    }

    ///
    /// Move assignment
    ///
    inline ByteVector& operator=(ByteVector&& other) noexcept
    {
        clear();
        new (this) ByteVector(other);
        return *this;
    }

    ///
    /// Destructor
    ///
    inline ~ByteVector() { clear(); }

    ///
    ///
    ///
    inline bool equals(ByteVector const& other) const
    {
        if (this == &other)
            return true;
        if (length() != other.length())
            return false;
        return ArrayRef<u8>(this->_data, this->length()).equals(ArrayRef<u8>(other._data, other.length()));
    }

    void append(u8 byte);
    void append(ByteVector const& other);

    UNSAFE_BEGIN void insert(size_t index, void const* bytes, size_t nBytes)
    {
        Assert(index <= _length, ASMS_INVALID(index));
        Assert(bytes, ASMS_INVALID(bytes));
        if (nBytes == 0) {
            return;
        }
        _ensureDataOnHeap();
        if (_length + nBytes >= _capacity) {
            auto prevCapacity = _capacity;
            _capacity = _capacity + (_capacity / 2);
            if (_capacity < prevCapacity + nBytes) {
                _capacity += nBytes + 2;
            }
            _reallocate();
        }
        _length += nBytes;
        // Shift elements after index forward to make room for the new elements
        memmove(&_data[index + nBytes], &_data[index], (_length - (index + nBytes)));
        // Copy values into array buffer
        memmove(&_data[index], bytes, nBytes);
        UNSAFE_END;
    }

    UNSAFE_BEGIN void erase(size_t index, size_t nBytes)
    {
        Assert(size_t((index + nBytes)) <= length());
        _ensureDataOnHeap();
        // Shift elements backwards into the space erased from
        memmove(&_data[index], &_data[index + nBytes], _length - (index + nBytes));
        // Refill the new space created at the end of the buffer with zeros
        memset(&_data[_length - nBytes], 0, nBytes);
        _length -= nBytes;
        if (_length > 16 && _length < (_capacity / 4)) {
            _capacity = max(16u, _capacity / 2);
            _reallocate();
        }
        UNSAFE_END;
    }

    void clear()
    {
        if (!_needheap && _data)
            delete[] _data;
        _capacity = 0;
        _length = 0;
        _data = nullptr;
    }

    FORCEINLINE size_t length() const { return _length; }
    FORCEINLINE bool empty() const { return _length == 0; }
    FORCEINLINE u8 const* data() const { return _data; }
    FORCEINLINE u8* data() { return _data; }
};


///
/// @brief StructVector is a "growable" piece of contiguous memory like ByteVector, except it is specialized to hold
/// structs of a specific type.
/// However, the structs stored in this collection must be trivial/standard layout types. Meaning:
///   * no custom copy constructors, destructors, assignment operators, or default constructors that do anything other
///   than set the data to zero
///     In other words,
///     * The copy constructor and assignment should be functionally equivalent to memcpy(structA, structB,
///     sizeof(structure))
///     * Default constructor should be functionally equivalent to memset(structure, 0, sizeof(structure))
///     * Destructor dos nothing
///
/// Why? Here's three reasons
/// #1 --
/// When appending, a Vector may reallocate its internal storage.
/// What is supposed to happen if the elements have custom destructors and copy constructors?
/// Also, what do you do if you erase an element, and the elements in front have to be shifted?
/// Are we going to call their copy constructors and destructors in the process of doing that?
/// Intuitively, it makes zero sense that appending objects to a vector should cause other objects to be 'deleted'.
///
/// #2 -- Symmetry -- The behavior of insert, erase, and others should be consistent across different collections.
///
/// In Java, if we replace LinkedList with ArrayList, there are zero differences in how the stored data is treated.
/// The collections ONLY DIFFER IN THE TIME COMPLEXITY OF CERTAIN OPERATIONS.
/// I would like to preserve this design philosophy in my implementation.
///
/// If we allow the Vector to store "complex objects" in-place, then as I mentioned before, append() has the possibility
/// of causing reallocation, which means all other objects are copy constructed and then destroyed. But with a linked
/// list, because it is not a contiguous memory sequence, append() will never cause that to happen. This is bad. What
/// does append do-- Append "inserts an element at the end of the collection." That's all it should do. An append should
/// do the same thing regardless of what type of collection it is for. Yet as we can see here, that is clearly not the
/// case. Append on a Vector has side effects on the stored data, but append on a LinkedList does not.
/// This is just bad software design, especially if we write generic algorithms where we assume the type of the
/// collection does not matter for performing a specific functions.
///
/// I want to create a software that is free of implicit behavior and hidden side effects.
/// Developers shouldn't't need to memorize "special cases."
///
/// #3 -- Performance -- memmove, memcpy, and memset are MUCH FASTER than a for-loop that "properly" copies objects.
/// So if our Vector used for-loops for the sake of supporting these complex objects, it would make performance bad for
/// elements like ints and pointers. Then I'd have to write specializations for those types to use memcpy instead, and
/// then there is 5x more code.
/// Banning complex objects makes the implementation simpler and allows for better performance.
///
template<typename T>
struct StructVector : private ByteVector
{
    StructVector() = default;

    FORCEINLINE void append(T const& value)
    {
        ByteVector::insert(ByteVector::length() - sizeof(value), &value, sizeof(value));
    }

    FORCEINLINE void repeat(size_t n)
    {
        while (n-- != 0) {
            ByteVector::append(*this);
        }
    }

    // TODO: implement
};


}  // namespace cm
#endif
