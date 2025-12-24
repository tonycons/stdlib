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


///
/// Iterator
///
template<typename Container, typename T>
class LinearIterator {
public:
    ///
    /// Iterator to the next element.
    ///
    constexpr auto next() const { return LinearIterator(_array, _index + 1); }

    ///
    /// Iterator to the previous element.
    ///
    constexpr auto prev() const { return LinearIterator(_array, _index - 1); }

    ///
    /// Checks if there exists an element before the current.
    ///
    constexpr bool hasPrev() const { return (_index > 0) && (_index < _array->length()); }

    ///
    /// Checks if there exists an element after the current.
    ///
    constexpr bool hasNext() const { return _index < usize(max(i64(_array->length()) - 1, 0)); }

    ///
    ///
    ///
    constexpr bool isNotEnd() const { return _index < _array->length(); }

    ///
    /// Moves the iterator to the next element.
    /// If there is no next element, the behavior is undefined.
    ///
    constexpr LinearIterator& operator++() { return (*this = this->next()); }

    ///
    /// Moves the iterator to the previous element.
    /// If there is no previous element, the behavior is undefined.
    ///
    constexpr LinearIterator& operator--() { return (*this = this->prev()); }

    ///
    /// Access the current element.
    ///
    constexpr T const& operator*() const noexcept { return _get(); }

    ///
    /// Access the current element.
    ///
    constexpr T& operator*() noexcept { return _get(); }

    ///
    /// Access the current element.
    ///
    constexpr T const* operator->() const noexcept { return &_get(); }

    ///
    /// Access the current element.
    ///
    constexpr T* operator->() noexcept { return &_get(); }

    ///
    /// Check if iterators are equal.
    ///
    constexpr bool operator==(LinearIterator const& other) const
        UNSAFE({ return _array->data() + _index == other._array->data() + other._index; });

    ///
    /// Inserts elements at the current position.
    /// The existing elements at the current position, and all after it, will be shifted forward by the amount of
    /// elements. The iterator will point to the same location in the array after insert().
    ///
    void insert(T const& value) { _array->insert(_index, value); }

    ///
    /// Removes the element at the current position.
    /// Any elements after the current element will be shifted back by one position.
    /// The iterator will point to the same location in the array after remove().
    ///
    void remove() { _array->erase(_index); }

    constexpr LinearIterator(Container const* array, size_t index)
        : _array(const_cast<Container*>(array)), _index(index)
    {}

    constexpr LinearIterator(Container* array, size_t index)
        : _array(array), _index(index)
    {}

    constexpr T const& _get() const { return (*_array)[_index]; }
    constexpr T& _get() { return (*_array)[_index]; }

private:
    Container* _array;
    T* _ptr;
    size_t _length;
    size_t _index;
};

///
/// Provides linear iterator functionality to a derived class.
/// A derived class using this component must implement:
/// length(): returns the number of elements
/// operator[]: accesses an element by index
/// insert(i, E): inserts an element E at index i
/// erase(i): Removes the element at index i
///
template<class Derived, class T>
class LinearIteratorComponent {
    using DP = Derived*;
    using DCP = Derived const*;

public:
    using Iterator = LinearIterator<Derived, T>;

    ///
    /// Iterator pointing to the first element.
    ///
    constexpr auto begin() { return Iterator(DP(this), 0); }

    ///
    /// Iterator pointing to the first element.
    ///
    constexpr auto begin() const { return Iterator(DCP(this), 0); }

    ///
    /// Iterator pointing to one past the last element.
    ///
    constexpr auto end() { return Iterator(DP(this), DCP(this)->length()); }

    ///
    /// Iterator pointing to one past the last element.
    ///
    constexpr auto end() const { return Iterator(DCP(this), DCP(this)->length()); }

    ///
    /// Iterator pointing to the first element. Same as begin().
    ///
    constexpr auto first() { return this->begin(); }

    ///
    /// Iterator pointing to the first element. Same as begin().
    ///
    constexpr auto first() const { return this->begin(); }

    ///
    /// Iterator pointing to the last element.
    ///
    constexpr auto last() { return Iterator(DP(this), DCP(this)->length() - 1); }

    ///
    /// Iterator pointing to the last element.
    ///
    constexpr auto last() const { return Iterator(DCP(this), DCP(this)->length() - 1); }

    ///
    /// @return an iterator to a specific index.
    ///
    constexpr auto at(size_t index)
    {
        DP(this)->operator[](index);  // This causes bounds checking to be performed
        return Iterator(this, index);
    }

    ///
    /// @return an iterator to a specific index.
    ///
    constexpr auto at(size_t index) const
    {
        DCP(this)->operator[](index);
        return Iterator(this, index);
    }

    ///
    /// Iterator that skips over certain elements.
    ///
    template<typename... Args>
    struct ExclusionIterator : public IEquatable<ExclusionIterator<Args...>>
    {
        constexpr ExclusionIterator(T const* ptr, T const* end, Tuple<Args...> const& skip)
            : skip_(skip), ptr_(ptr), end_(end)
        {

            while (shouldSkip_() && ptr_ < end_)
                ptr_++;
        }

        constexpr T const& operator*() const noexcept { return *this->ptr_; }

        constexpr T& operator*() noexcept { return const_cast<T&>(*this->ptr_); }

        constexpr bool equals(T c) const noexcept { return *ptr_ == c; }

        constexpr bool equals(ExclusionIterator<Args...> const& i) const noexcept
        {
            return ptr_ == i.ptr_ || (ptr_ >= end_ && i.ptr_ >= end_);
        }

        constexpr ExclusionIterator& operator++() noexcept UNSAFE({
            do {
                if (ptr_ >= end_)
                    break;
                ptr_++;
            } while (shouldSkip_());
            return *this;
        });

        constexpr ExclusionIterator operator++(int) noexcept UNSAFE({
            do {
                if (ptr_ >= end_)
                    break;
                ptr_++;
            } while (shouldSkip_());
            return *this;
        });

    private:
        Tuple<Args...> skip_;
        T const* ptr_;
        T const* end_;

        constexpr bool shouldSkip_() const noexcept UNSAFE({
            return [&]<int N_>() {
                auto _ = [&]<int N>(auto ref) {
                    if constexpr (N >= sizeof...(Args)) {
                        return false;
                    } else {
                        if (*ptr_ == skip_.template get<N>())
                            return true;

                        return ref.template operator()<N + 1>(ref);
                    }
                };
                return _.template operator()<N_>(_);
            }.template operator()<0>();
        });
    };

    template<typename... Args>
    struct Excluder
    {
        constexpr Excluder(LinearIteratorComponent const& b, Args... args)
            : b_(b), skip_(args...)
        {}

        constexpr auto begin() const
        {
            return ExclusionIterator<Args...>(
                static_cast<Derived const&>(b_).iterable_begin(), static_cast<Derived const&>(b_).iterable_end(),
                skip_);
        }

        constexpr auto end() const
        {
            auto const _ = static_cast<Derived const&>(b_).iterable_end();
            return ExclusionIterator<Args...>(_, _, skip_);
        }

    private:
        LinearIteratorComponent const& b_;
        Tuple<Args...> skip_;
    };

    ///
    /// Iterate over the container, EXCLUDING certain elements.
    ///
    template<typename... Args>
    requires (__is_same(Args, T) && ...)
    Excluder<Args...> exclude(Args... args) const
    {
        return Excluder<Args...>(*this, args...);
    }

    template<typename... Args>
    struct IncludingIterator
    {

        constexpr IncludingIterator(T const* ptr, T const* end, Tuple<Args...> const& include)
            : include_(include), ptr_(ptr), end_(end)
        {
            while (shouldSkip_() && ptr_ < end_)
                ptr_++;
        }

        constexpr operator T() const { return ptr_ >= end_ ? *end_ : *ptr_; }

        constexpr IncludingIterator operator*() const noexcept { return *this; }

        constexpr bool equals(T c) const noexcept { return *ptr_ == c; }

        constexpr bool equals(IncludingIterator const& i) const noexcept
        {
            return ptr_ == i.ptr_ || (ptr_ >= end_ && i.ptr_ >= end_);
        }

        constexpr IncludingIterator& operator++() noexcept
        {
            do {
                if (ptr_ >= end_)
                    break;
                ptr_++;
            } while (shouldSkip_());
            return *this;
        }

        constexpr IncludingIterator operator++(int) noexcept
        {
            do {
                if (ptr_ >= end_)
                    break;
                ptr_++;
            } while (shouldSkip_());
            return *this;
        }

    private:
        Tuple<Args...> include_;
        T const* ptr_;
        T const* end_;

        constexpr bool shouldSkip_() const noexcept
        {
            return [&]<int N_>() {
                auto _ = [&]<int N>(auto ref) {
                    if constexpr (N >= sizeof...(Args)) {
                        return true;
                    } else {
                        if (*ptr_ == include_.template get<N>())
                            return false;

                        return ref.template operator()<N + 1>(ref);
                    }
                };
                return _.template operator()<N_>(_);
            }.template operator()<0>();
        }
    };

    template<typename... Args>
    struct Includer
    {

        constexpr Includer(LinearIteratorComponent const& b, Args... args)
            : b_(b), skip_(args...)
        {}

        constexpr auto begin() const
        {
            return IncludingIterator<Args...>(
                static_cast<Derived const&>(b_).iterable_begin(), static_cast<Derived const&>(b_).iterable_end(),
                skip_);
        }

        constexpr auto end() const
        {
            auto end = static_cast<Derived const&>(b_).iterable_end();
            return IncludingIterator<Args...>(end, end, skip_);
        }

    private:
        LinearIteratorComponent const& b_;
        Tuple<Args...> skip_;
    };

    template<typename... Args>
    requires (__is_same(Args, T) && ...)
    constexpr Includer<Args...> include(Args... args) const
    {
        return Includer<Args...>(*this, args...);
    }
};

}  // namespace cm

#endif
