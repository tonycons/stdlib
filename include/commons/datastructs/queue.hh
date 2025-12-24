#pragma once
#include "array.hh"


namespace cm {

///
/// A fixed-capacity stack data structure.
/// @tparam T Element type
/// @tparam Capacity Maximum number of elements the stack can hold
///
template<typename T, usize Capacity = ARRAY_LENGTH_UNSPECIFIED>
struct FixedQueue  //
    : public Iterable<FixedQueue<T, Capacity>>,
      public LinearIteratorComponent<FixedQueue<T, Capacity>, Optional<T>>
{
private:
    Array<Optional<T>, Capacity> _array;
    usize _next = 0;
    usize _first = 0;
    usize _length = 0;

public:
    ///
    /// Create a queue with runtime-specified capacity from an initializer list.
    /// @param cap The capacity of the queue (because having the size of the queue limited to the number of initial
    /// elements would be useless)
    /// @param initializer the initial values to place in the queue
    ///
    inline FixedQueue(usize cap, std::initializer_list<T> const& initializer)
        requires (Capacity == ARRAY_LENGTH_UNSPECIFIED)
        : _array([&] {
              Assert(cap >= initializer.size(), ASMS_INVALID(cap));
              return cap;
          }())
    {
        for (auto const& value : initializer) {
            push(value);
        }
    }

    ///
    /// Create a queue with compile-time specified capacity from an initializer list.
    /// @param initializer The initial values to place in the queue
    ///
    constexpr inline FixedQueue(std::initializer_list<T> const& initializer)
        requires (Capacity != ARRAY_LENGTH_UNSPECIFIED)
    {
        Assert(initializer.size() <= Capacity, ASMS_INVALID(initializer));
        for (auto const& value : initializer) {
            push(value);
        }
    }

    void push(T const& value)
    {
        _array[_next] = value;

        if (_next == _array.length() - 1) {
            _next = 0;
        } else {
            ++_next;
        }
        _length = ::cm::min(_length + 1, _array.length());
    }

    Optional<T> pop()
    {
        if (length() == 0) {
            return None;
        } else {
            auto tmp = _first;
            if (_first == _array.length() - 1) {
                _first = 0;
            } else {
                ++_first;
            }
            --_length;

            Optional<T> opt = _array[tmp];
            _array[tmp] = None;
            return opt;
        }
    }

    inline Optional<T> const& operator[](usize index) const [[clang::lifetimebound]]
    {
        Assert(index < length(), ASMS_BOUNDS);
        auto tmp = _first;
        if (_first + index > _array.length()) {
            tmp = (_first + index) % _array.length();
        }
        return _array[tmp];
    }

    inline Optional<T>& operator[](usize index) [[clang::lifetimebound]]
    {
        return const_cast<Optional<T>&>(static_cast<FixedQueue const&>(*this)[index]);
    }

    inline usize length() const { return _length; }
    inline usize capacity() const { return _array.length(); }
};

}  // namespace cm
