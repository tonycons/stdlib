#pragma once

#include <Commons/core.hh>
#include <Commons/core.hh>
#include <Commons/core.hh>
#include <cstring>
#include "Commons/datastructs/ArrayIterator.hh"
#include <Commons/core.hh>
#include <Commons/core.hh>
#include <Commons/core.hh>
#include "Commons/datastructs/Collection.hh"
#include "FixedArray.hh"


namespace cm {
template<IsPrimitiveData T>
class ResizableArray;

namespace CollectionType {
template<IsPrimitiveData T>
class ResizableArray {
public:
    using ItsElementType = T;

    template<typename SomeType = T>
    using ItsType = ::cm::ResizableArray<SomeType>;
};
}

/**
 * Resizable array.
 * @tparam T Array element type
 */
template<IsPrimitiveData T>
class ResizableArray : public Collection<CollectionType::ResizableArray<T>>,
                       // public IEquatable<ResizableArray<T>>,
                       //  protected ChecksumProtection<ResizableArray<T>>,
                       public LinearIteratorComponent<ResizableArray<T>, T>,
                       public SafeBinaryMoveable {
protected:
    T* _data = nullptr;
    size_t _length = 0;
    size_t _capacity = 0;
    bool _needheap = false;


public:
    using ThisType = ResizableArray<T>;
    // using ChecksumProtection<ThisType>::checksumUpdate;
    // using ChecksumProtection<ThisType>::checksumValidate;
    //  using typename ChecksumProtection<ThisType>::ValidateAndUpdateChecksum;
    constexpr static auto NullTerminator = T{};


    /**
     * Default constructor.
     */
    constexpr ResizableArray() = default;

    /**
     * Constructor from initializer list.
     * @param lst Initializer list
     */
    ResizableArray(std::initializer_list<T> const& lst) noexcept
    {
        Assert(lst.begin(), ASMS_INVALID(lst.data()));
        if (Ptr::isRomData(lst.begin())) {
            _data = const_cast<T*>(lst.begin());
            _length = _capacity = static_cast<size_t>(lst.size());
            _needheap = true;
        } else {
            insert(0, lst);
        }
    }

    /**
     * Constructor from pointer to memory.
     * @param ptr Pointer
     * @param len Number of elements pointed to
     */
    ResizableArray(T const* ptr, size_t len) noexcept
    {
        Assert(ptr, ASMS_INVALID(ptr));
        Assert(len, ASMS_INVALID(len));
        if (Ptr::isRomData(ptr)) {
            _data = const_cast<T*>(ptr);
            _length = _capacity = len;
            _needheap = true;
        } else {
            insert(0, ptr, len);
        }
    }

    /**
     * Copy constructor.
     * @param other ResizableArray
     */
    ResizableArray(ResizableArray<T> const& other) noexcept
    {
        if (Ptr::isRomData(other._data)) {
            _data = const_cast<T*>(other._data);
            _length = _capacity = other._length;
            _needheap = true;
        } else {
            if (other._data) {
                insert(0, other._data, other._length);
            }
        }
    }

    /**
     * Move constructor.
     */
    ResizableArray(ThisType&& other) noexcept
    {
        _data = other._data;
        _length = other._length;
        _capacity = other._capacity;
        _needheap = other._needheap;
        other._data = nullptr;
    }

    /**
     * Copy constructor.
     * @param other FixedArray
     */
    template<unsigned L>
    ResizableArray(FixedArray<T, L> const& other) noexcept
    {
        if (Ptr::isRomData(other._data)) {
            _data = const_cast<T*>(other._data);
            _length = _capacity = other._length;
            _needheap = true;
        } else {
            insert(0, other.data(), other.length());
        }
    }

    /**
     * Assignment operator.
     * @param other ResizableArray
     */
    auto& operator=(ResizableArray<T> const& other) noexcept
    {
        clear();
        new (this) ResizableArray<T>(other);
        return *this;
    }

    /**
     * Move assignment operator.
     * @param other ResizableArray
     */
    auto& operator=(ResizableArray<T>&& other) noexcept
    {
        clear();
        new (this) ResizableArray<T>(other);
        return *this;
    }

    /**
     * Assignment operator.
     * @param other FixedArray
     */
    template<unsigned L>
    auto& operator=(FixedArray<T, L> const& other) noexcept
    {
        clear();
        new (this) ResizableArray<T>(other);
        return *this;
    }

    /**
     * Destructor.
     */
    virtual ~ResizableArray() { clear(); }

    /**
     * Access an element by position.
     * @param index Position in range [0, length() - 1]
     * @return Reference to the element
     */
    constexpr T& operator[](IsInteger auto index)
    {
        _ensureDataOnHeap();
        Assert(index >= 0 && size_t(index) < length(), ASMS_INVALID(index));
        UNSAFE(return _data[index];)
    }

    /**
     * Access an element by position.
     * @param index Position in range [0, length() - 1]
     * @return Const reference to the element
     */
    constexpr T const& operator[](IsInteger auto index) const
    {
        Assert(index >= 0 && size_t(index) < length(), ASMS_INVALID(index));
        UNSAFE(return _data[index];)
    }

    /**
     * Performs a deep comparison to another array.
     * @param other Another array
     * @return bool
     */
    constexpr bool equals(ResizableArray<T> const& other) const
    {
        if (this == &other)
            return true;
        if (length() != other.length())
            return false;
        return compareEq<T>(_data, other._data, _length);
    }


    /**
     * Inserts an element at the current position.
     * @param index position
     * @param value Element to append
     */
    auto& insert(size_t index, T const& value) { return insert(index, &value, 1); }

    /**
     * Inserts elements at the current position.
     * @param index position
     * @param values List containing elements to append
     */
    auto& insert(size_t index, std::initializer_list<T> const& values)
    {
        return insert(index, values.begin(), static_cast<size_t>(values.size()));
    }

    /**
     * Inserts elements at the current position.
     * @param index position
     * @param values List containing elements to append
     */
    template<unsigned N>
    auto& insert(size_t index, T const (&values)[N])
    {
        return insert(index, values, N);
    }

    /**
     * Inserts elements at the current position.
     * @param index position
     * @param values List containing elements to append
     */
    auto& insert(size_t index, auto const& values) { return insert(index, values.data(), values.length()); }

    /**
     * Inserts elements at the current position.
     * Note: If position == length(), then it will append the elements.
     * @param index position
     * @param values Pointer to elements
     * @param count number of elements
     */
    UNSAFE_BEGIN
    auto& insert(size_t index, T const* values, size_t count)
    {
        Assert(index <= _length, ASMS_INVALID(index));
        Assert(values, ASMS_INVALID(values));
        Assert(count, ASMS_INVALID(count));
        _ensureDataOnHeap();
        if (_length + count >= _capacity) {
            auto prevCapacity = _capacity;
            _capacity = _capacity + (_capacity / 2);
            if (_capacity < prevCapacity + count) {
                _capacity += count + 2;
            }
            _reallocate();
        }
        _length += count;
        // Shift elements after index forward to make room for the new elements
        memmove(&_data[index + count], &_data[index], sizeof(T) * (_length - (index + count)));
        // Copy values into array buffer
        memmove(&_data[index], values, sizeof(T) * count);
        return *this;
    }
    UNSAFE_END

    /**
     * Remove a series of elements starting at a position.
     * @param index position
     * @param count .
     */
    UNSAFE_BEGIN
    auto& erase(size_t index, size_t count = 1)
    {
        Assert(static_cast<size_t>((index + count)) <= length());
        _ensureDataOnHeap();
        // Shift elements backwards into the space erased from
        moveDataToNewRegion(&_data[index], &_data[index + count], (_length - (index + count)));
        // Refill the new space created at the end of the buffer with zeros
        memset(&_data[_length - count], 0, sizeof(T) * count);

        _length -= count;
        if (_length > 16 && _length < (_capacity / 4)) {
            _capacity = max(16u, _capacity / 2);
            _reallocate();
        }
        return *this;
    }
    UNSAFE_END

    /**
     *
     */
    auto& clear()
    {
        if (!_needheap && _data) {
            delete[] _data;
        }
        _capacity = 0;
        _length = 0;
        _data = nullptr;
        return *this;
    }

    constexpr inline T* data() { return _ensureDataOnHeap(), &_data[0]; }

    constexpr inline T const* data() const { return &_data[0]; }

    constexpr inline size_t length() const { return _length; }

    constexpr inline bool empty() const { return _length == 0; }

    constexpr inline size_t sizeBytes() const { return length() * sizeof(T); }


protected:
    // T _initial_buffer;
    //  u8 _initial_buffer[16];

    inline void _ensureDataOnHeap()
    {
        if (_needheap) {
            _makeHeapAllocated();
        }
    }

    void _makeHeapAllocated()
    {
        Assert(Ptr::isRomData(_data), ASMS_BAD_CIRCUMSTANCE);
        auto old = _data;
        _data = new T[_capacity];
        moveDataToNewRegion<T>(_data, old, _capacity);
        _needheap = false;
    }

    void _reallocate() UNSAFE({
        // CM_CHECKSUM_PROTECTED;
        Assert(_capacity > 0, ASMS_BUG);
        auto old = _data;
        _data = new T[size_t(_capacity)];
        moveDataToNewRegion<T>(_data, old, _length);
        delete[] old;
    });
};

}  // namespace cm
