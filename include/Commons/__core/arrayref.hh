#pragma once
#ifdef __inline_core_header__
#include <sys/types.h>


namespace cm {

///
/// An abstraction over array indices that allows negative indices to index from the end of the array like in python
///
struct Index : Union<ssize_t, size_t>
{
    using Union<ssize_t, size_t>::Union;

    NODISCARD FORCEINLINE constexpr auto computeUnchecked(auto const& a) const
    {
        size_t i_ = match(
            [&](size_t k) { return k; },
            [&](ssize_t k) {  // Possible negative index
                return k < 0 ? size_t(ssize_t(a.length()) + k) : size_t(k);
            });
        return i_;
    }

    NODISCARD FORCEINLINE constexpr auto compute(auto const& a) const
    {
        size_t i = computeUnchecked(a);
        Assert(i < a.length(), ASMS_BOUNDS);
        return i;
    }

    NODISCARD FORCEINLINE constexpr usize assertPositive() const
    {
        size_t i = match(
            [&](size_t k) { return k; },
            [&](ssize_t k) {
                Assert(k >= 0, ASMS_PARAMETER);
                return size_t(k);
            });
        return i;
    }
};


///
/// A non-owning reference to an array
///
template<typename T>
struct ArrayRef
{
    T* _ptr;
    size_t _length;

    // ArrayRef(auto const& v)
    //     : _ptr(v.data()), _length(v.length())
    // {}


    FORCEINLINE explicit constexpr ArrayRef(T* ptr_, size_t length_) noexcept
        : _ptr(ptr_), _length(length_)
    {}

    /// Constructor from initializer list
    FORCEINLINE constexpr ArrayRef(std::initializer_list<T> const& v) noexcept
        : _ptr(const_cast<T*>(v.begin())), _length(v.size())
    {}

    /// Constructor from character literal
    template<unsigned N>
    FORCEINLINE constexpr ArrayRef(T const (&literal)[N]) noexcept
        : _ptr(const_cast<T*>(literal)), _length(N)
    {}

    NODISCARD FORCEINLINE constexpr T const& operator[](Index const& i) const
    {
        size_t i_ = i.compute(*this);
        Assert(i_ < length(), ASMS_BOUNDS);
        UNSAFE({ return _ptr[i_]; });
    }


    NODISCARD FORCEINLINE constexpr T const& operator()(Index const& i) const
    {
        size_t i_ = i.computeUnchecked(*this);
        UNSAFE({ return _ptr[i_]; });
    }

    NODISCARD FORCEINLINE constexpr auto length() const noexcept { return _length; }
    NODISCARD FORCEINLINE constexpr auto sizeBytes() const noexcept { return _length * sizeof(T); }

    // First element.
    NODISCARD FORCEINLINE constexpr T const* begin() const noexcept { return _ptr; }
    // One past the last element.
    NODISCARD FORCEINLINE constexpr T const* end() const noexcept { return UNSAFE(_ptr + _length); }

    NODISCARD FORCEINLINE constexpr T const* data() const noexcept { return _ptr; }
    NODISCARD FORCEINLINE constexpr T* data() noexcept { return _ptr; }


    constexpr static void outputString(ArrayRef<T> const& a, auto const& out);


    constexpr T const& min(this auto&& self)
    {
        T m = self[0];
        for (usize i = 1; i < self.length(); i++) {
            m = ::cm::min(m, self(i));
        }
        return m;
    }
};

template<typename T>
ArrayRef(T*, size_t) -> ArrayRef<T>;

template<typename T>
ArrayRef(std::initializer_list<T> const&) -> ArrayRef<T>;

template<typename T, unsigned N>
ArrayRef(T const (&literal)[N]) -> ArrayRef<T>;


using StringRef = ArrayRef<char>;

}  // namespace cm
#endif
