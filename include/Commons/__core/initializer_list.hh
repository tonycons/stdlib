#pragma once
#ifdef __inline_core_header__
#include <string.h>


namespace std {

/// initializer_list
template<class E>
class initializer_list {
public:
    typedef E value_type;
    typedef E const& reference;
    typedef E const& const_reference;
    typedef __SIZE_TYPE__ size_type;
    typedef E const* iterator;
    typedef E const* const_iterator;

private:
    E const* _array;
    __SIZE_TYPE__ _len;

    // The compiler can call a private constructor.
    constexpr initializer_list(E const* a, __SIZE_TYPE__ l)
        : _array(a), _len(l)
    {}

public:
    constexpr initializer_list() noexcept
        : _array(nullptr), _len(0)
    {}

    // Number of elements.
    constexpr auto size() const noexcept { return _len; }

    // First element.
    constexpr E const* begin() const noexcept { return _array; }

    // One past the last element.
    constexpr E const* end() const noexcept { return UNSAFE(begin() + size()); }
};

/**
 *  @brief  Return an iterator pointing to the first element of the initializer_list.
 *  @param  ils  Initializer list.
 *  @relates initializer_list
 */
template<class T>
constexpr T const* begin(initializer_list<T> ils) noexcept
{
    return ils.begin();
}

/**
 *  @brief  Return an iterator pointing to one past the last element of the initializer_list.
 *  @param  ils  Initializer list.
 *  @relates initializer_list
 */
template<class T>
constexpr T const* end(initializer_list<T> ils) noexcept
{
    return ils.end();
}

}  // namespace std


namespace cm {


template<typename T>
class RefWrapper {
public:
    FORCEINLINE RefWrapper(T& x)
        : ptr(&x)
    {}

    FORCEINLINE operator T&() { return *ptr; }
    FORCEINLINE operator T const&() const { return *ptr; }
    FORCEINLINE T* operator->() { return ptr; }
    FORCEINLINE T const* operator->() const { return ptr; }

private:
    T* ptr;
};

template<typename T>
class ConstRefWrapper {
public:
    FORCEINLINE ConstRefWrapper(T const& x)
        : ptr(&x)
    {}

    FORCEINLINE operator T&() { return *ptr; }
    FORCEINLINE operator T const&() const { return *ptr; }
    FORCEINLINE T* operator->() { return ptr; }
    FORCEINLINE T const* operator->() const { return ptr; }

private:
    T const* ptr;
};


template<typename T, unsigned N>
struct CVArray
{
    T _data[N];

    constexpr CVArray() = default;

    constexpr CVArray(auto... values) requires (sizeof...(values) <= N)
        : _data{static_cast<T>(values)...}
    {}

    constexpr CVArray(T const (&values)[N])
        : _data(values)
    {}

    FORCEINLINE constexpr T& operator[](auto index)
    {
        UNSAFE({ return _data[index]; });
    }

    FORCEINLINE constexpr T const& operator[](auto index) const
    {
        UNSAFE({ return _data[index]; });
    }
};

template<typename T, unsigned N>
CVArray(T const (&)[N]) -> CVArray<T, N>;


/**
 * @brief Moves data (which may be overlapping) to a new memory location, mostly used in ResizableArray

 * If the data "safe to move" (i.e. is trivially copyable or inherits from SafeBinaryMoveable), it is copied via
 * memmove. Otherwise, it is copied by using copy constructors.

 * "Safe to move" Means that for a given instance of type T, changing the instance's location in memory would not mess
 * up its internal state.

 * Node structures or structures using self-reference are generally not safe to move. Consider the
 * struct Foo { Foo() : ptr(&x) {}; int x; int *ptr; }
 * Moving an instance of Foo in memory would invalidate ptr.

 * @param dst Destination pointer to a series of uninitialized T.
 * @param src Source pointer to a series of instances of T.
 * @param n The number of elements to copy
 */
UNSAFE_BEGIN
template<typename T>
constexpr void moveDataToNewRegion(T* dst, T const* src, size_t n)
{
    if consteval {
        for (__SIZE_TYPE__ i = 0; i < n; i++) {
            new (dst + i) T(src[i]);
        }
    } else {
        if constexpr (TriviallyCopyConstructible<T> || IsDerivedFrom<SafeBinaryMoveable, T>) {
            __builtin_memmove(dst, src, sizeof(T) * n);
        } else {
            for (__SIZE_TYPE__ i = 0; i < n; i++) {
                new (dst + i) T(src[i]);
            }
        }
    }
}

template<typename T>
constexpr bool compareEq(T const* a, T const* b, size_t n)
{
    if consteval {
        for (size_t i = 0; i < n; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
    } else {
        if constexpr (IsPrimitiveData<T>) {
            return memcmp(a, b, n * sizeof(T)) == 0;
        } else {
            for (size_t i = 0; i < n; i++) {
                if (a[i] != b[i]) {
                    return false;
                }
            }
        }
    }
    return true;
}

template<typename T>
constexpr void DefaultInitialize(T const* a, size_t n)
{
    if consteval {
        for (size_t i = 0; i < n; i++) {
            a[i] = T{};
        }
    } else {
        if constexpr (IsPrimitiveData<T>) {
            memset(a, 0, sizeof(T) * n);
        } else {
            for (size_t i = 0; i < n; i++) {
                a[i] = T{};
            }
        }
    }
}

UNSAFE_END

}  // namespace cm
#endif
