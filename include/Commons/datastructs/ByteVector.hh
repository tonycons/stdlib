#pragma once
#include <Commons/core.hh>
#include <Commons/core.hh>
#include <Commons/core.hh>


namespace cm {


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
///
/// IMPORTANT - This is meant to be used for trivial and standard layout types.
/// Meaning- no special copy constructors
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
/// If we allow the Vector to store "complex objects," then as I mentioned before, append() has the possibility of
/// causing reallocation, which means all other objects are copy constructed and then destroyed. But with a linked list,
/// because it is not a contiguous memory sequence, append() will never cause that to happen. This is bad. What does
/// append do-- Append "inserts an element at the end of the collection." That's all it should do. An append should do
/// the same thing regardless of what type of collection it is for. Yet as we can see here, that is clearly not the
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

// struct StructVector : private ByteVector
// {
//     StructVector() = default;


// FORCEINLINE void append(int const& value)
// {
//     ByteVector::insert(ByteVector::length() - sizeof(value), &value, sizeof(value));
// }

// FORCEINLINE void repeat(size_t n)
// {
//     while (n-- != 0) {
//         ByteVector::append(*this);
//     }
// }

// FORCEINLINE void
// };


// static_assert(TriviallyDestructible<ByteVector>);
// static_assert(TriviallyCopyConstructible<ByteVector>);
// static_assert(TriviallyDefaultConstructible<ByteVector>);

// static_assert(IsPrimitiveData<ByteVector>);


}  // namespace cm
