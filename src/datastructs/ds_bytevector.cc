#include <Commons/datastructs/ByteVector.hh>
#include <Commons/core.hh>
#include <Commons/core.hh>
#include <string.h>


namespace cm {

ByteVector::ByteVector(void const* ptr, usize len) noexcept
{
    Assert(ptr, ASMS_INVALID(ptr));
    Assert(len, ASMS_INVALID(len));

    if (Ptr::isRomData(ptr)) {
        _data = static_cast<u8*>(const_cast<void*>(ptr));
        _length = _capacity = len;
        _needheap = true;
    } else {
        insert(0, ptr, len);
    }
}

ByteVector::ByteVector(ByteVector const& other) noexcept
    : ByteVector()
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

ByteVector::ByteVector(ByteVector&& other) noexcept
{
    _data = other._data;
    _length = other._length;
    _capacity = other._capacity;
    _needheap = other._needheap;
    other._data = nullptr;
}

ByteVector& ByteVector::operator=(ByteVector const& other) noexcept
{
    clear();
    new (this) ByteVector(other);
    return *this;
}

ByteVector& ByteVector::operator=(ByteVector&& other) noexcept
{
    clear();
    new (this) ByteVector(other);
    return *this;
}

ByteVector::~ByteVector() { clear(); }

bool ByteVector::equals(ByteVector const& other) const
{
    if (this == &other)
        return true;
    if (length() != other.length())
        return false;
    return memcmp(this->_data, other._data, this->length()) == 0;
}

// void ByteVector::append(u8 byte) {}

// void ByteVector::append(ByteVector const& other) {}

UNSAFE_BEGIN
void ByteVector::insert(size_t index, void const* bytes, size_t nBytes)
{
    Assert(index <= _length, ASMS_INVALID(index));
    Assert(bytes, ASMS_INVALID(bytes));
    Assert(nBytes, ASMS_INVALID(nBytes));
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
}

void ByteVector::erase(size_t index, size_t nBytes)
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
}

void ByteVector::clear()
{
    if (!_needheap && _data)
        delete[] _data;
    _capacity = 0;
    _length = 0;
    _data = nullptr;
}

void ByteVector::_ensureDataOnHeap()
{
    if (!_needheap)
        return;
    Assert(Ptr::isRomData(_data), ASMS_BAD_CIRCUMSTANCE);
    auto old = _data;
    _data = new u8[_capacity];
    memmove(_data, old, _capacity);
    _needheap = false;
}

void ByteVector::_reallocate()
{
    Assert(_capacity > 0, ASMS_BUG);
    auto old = _data;
    _data = new u8[size_t(_capacity)];
    memmove(_data, old, _length);
    delete[] old;
}

UNSAFE_END

}  // namespace cm
