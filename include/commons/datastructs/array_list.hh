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
#include <commons/core.hh>
#include <commons/core.hh>


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
    void _ensureDataOnHeap();
    void _reallocate();

public:
    constexpr ByteVector() = default;
    ByteVector(void const* data, usize len) noexcept;
    ByteVector(ByteVector const& other) noexcept;
    ByteVector(ByteVector&& other) noexcept;
    ByteVector& operator=(ByteVector const& other) noexcept;
    ByteVector& operator=(ByteVector&& other) noexcept;
    ~ByteVector();

    bool equals(ByteVector const& other) const;
    void append(u8 byte);
    void append(ByteVector const& other);
    void insert(size_t index, void const* data, size_t len);
    void erase(size_t index, size_t nBytes);
    void clear();

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
