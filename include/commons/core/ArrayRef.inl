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
/// A non-owning reference to an array
/// It uses clang's typestate and lifetime annotations to attempt to issue warnings on incorrect usage. For a given
/// instance of ArrayRef, "unconsumed" is the normal state. Being in the "consumed" state indicates that it is
/// invalidated, and making copies of the ArrayRef is unsafe.
///
template<typename T>
struct [[clang::consumable(unconsumed)]] ArrayRef : IArray<T>
{
private:
    T const* _ptr = nullptr;
    size_t _length = 0;

public:
    ///
    /// Default constructor.
    ///
    [[clang::return_typestate(unconsumed)]]
    constexpr ArrayRef() noexcept = default;

    ///
    /// Constructor from pointer and length
    /// @param ptr_ The pointer
    /// @param length_ The length
    ///
    [[clang::return_typestate(unconsumed)]]
    explicit constexpr ArrayRef(T const* ptr_, size_t length_) noexcept
        : _ptr(ptr_), _length(length_)
    {}

    ///
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
    ///
    [[clang::return_typestate(consumed)]]
    constexpr ArrayRef([[clang::lifetimebound]] ::std::initializer_list<T> const& v) noexcept
        : _ptr(const_cast<T*>(v.begin())), _length(v.size())
    {}

    ///
    /// Constructor from character literal
    ///
    template<unsigned N>
    [[clang::return_typestate(unconsumed)]] constexpr ArrayRef([[clang::lifetimebound]] T const (&literal)[N]) noexcept
        : _ptr(const_cast<T*>(literal)), _length(N)
    {}

    [[clang::callable_when(unconsumed)]]
    constexpr ArrayRef([[clang::param_typestate(unconsumed)]] [[clang::lifetimebound]] ArrayRef const&) noexcept =
        default;

    [[clang::callable_when(unconsumed)]]
    constexpr ArrayRef([[clang::param_typestate(unconsumed)]] [[clang::lifetimebound]] ArrayRef&&) noexcept = default;

    // Explicitly defaulted move assignment operator (C++11 and later)
    [[clang::callable_when(unconsumed)]]
    constexpr ArrayRef&
    operator=([[clang::param_typestate(unconsumed)]] [[clang::lifetimebound]] ArrayRef&&) noexcept = default;

    [[clang::callable_when(unconsumed)]]
    constexpr ArrayRef&
    operator=([[clang::param_typestate(unconsumed)]] [[clang::lifetimebound]] ArrayRef const&) noexcept = default;


    ///
    /// Index operator. Performs bounds checking.
    /// @param i the index
    ///
    constexpr T const& operator[](Index const& i) const override
    {
        auto const i_ = i.compute(*this);
        Assert(i_ < length(), ASMS_BOUNDS);
        UNSAFE({ return _ptr[i_]; });
    }

    ///
    /// Index operator that does not perform bounds checking.
    ///
    constexpr T const& operator()(Index const& i) const override
    {
        auto const i_ = i.computeUnchecked(*this);
        UNSAFE({ return _ptr[i_]; });
    }

    ///
    /// Returns how many elements are in the array.
    ///
    constexpr usize length() const override { return _length; }

    ///
    /// Returns a pointer to the array data.
    ///
    constexpr T const* data() const override { return _ptr; }
};

// Deduction guides for ArrayRef

template<typename T>
ArrayRef(T*, size_t) -> ArrayRef<T>;
template<typename T>
ArrayRef(::std::initializer_list<T>&&) -> ArrayRef<T>;
template<typename T, unsigned N>
ArrayRef(T const (&literal)[N]) -> ArrayRef<T>;

}  // namespace cm
#endif
