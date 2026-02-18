#pragma once

#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else


namespace cm {


template<typename T>
constexpr inline auto DefaultSortPredicate = [](T const& x, T const& y) -> int {
    if (x < y) {
        return -1;
    } else if (x == y) {
        return 0;
    } else {
        return 1;
    }
};

namespace Sort {

template<typename T>
constexpr inline auto Ascending = DefaultSortPredicate<T>;
}

namespace Search {

template<typename T>
constexpr inline auto DefaultComparator = [](T const& a, T const& b) -> bool {
    return a == b;
};

template<typename T>
struct Result
{
    i64 index;
    T value;
    Result(i64 index, T value)
        : index(index), value(value)
    {}
};

template<typename T>
Result(i64, T) -> Result<T>;

/**
 * @brief Uses linear search to find an occurence of *value* in *container*.
 * @param container The container
 * @param value The value
 * @param cmp A comparator, operator== by default
 * @return The index if found, otherwise -1
 */
template<typename T, typename F = decltype(DefaultComparator<T>)>
Optional<Result<T>> linearSearch(auto const& container, T const& value, F cmp = DefaultComparator<T>)
{
    long i = 0;
    for (auto const& cont_value : container) {
        if (cmp(cont_value, value)) {
            return Result(i, cont_value);
        }
        i++;
    }
    return None;
}


/**
 * @brief Uses binary search to find an occurence of *value* in *container*.
 * @return The index of the target element if found, otherwise -1
 * @param value The value
 * @param cmp A sorting comparison predicate,
 * for a given (x, y) should return a negative integer if x < y, zero if x == y, and a positive integer if x > y.
 * @note
 https://research.google/blog/extra-extra-read-all-about-it-nearly-all-binary-searches-and-mergesorts-are-broken/
 */
template<typename T, typename F = decltype(DefaultComparator<T>)>
Optional<Result<T>> binary(T const& value, ArrayRef<T> const& container, F cmp = DefaultComparator<T>)
{
    i64 low = 0;
    i64 high = i64(container.length()) - 1;

    while (low <= high) {
        // Calculate mid to prevent potential integer overflow
        i64 mid = (low + (high - low) / 2);

        if (cmp(container[mid], value) == 0) {
            return Result(mid, container[mid]);
        } else if (cmp(container[mid], value) < 0) {
            low = mid + 1;  // Target is in the right half
        } else {
            high = mid - 1;  // Target is in the left half
        }
    }
    return None;
}
//};

// template<typename T, typename F>
// Binary(T const&, ArrayRef<T> const&, F) -> Binary<T>;


}  // namespace Search

}  // namespace cm
#endif
