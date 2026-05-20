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
#include <commons/core/typeTraits.hh>
#include <generator>  // IWYU pragma: keep
#include <memory>

namespace cm {

///
/// This somehow has much better codegen than std::generator.. I don't even know how
///
template<typename T>
struct Generator : NonCopyable
{
    struct promise_type
    {
        T const* value;

        constexpr auto get_return_object()
        {
            return Generator<T>{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        constexpr auto yield_value(T const& v)
        {
            value = std::addressof(v);
            return std::suspend_always{};
        }

        constexpr auto return_void() {}
        constexpr auto initial_suspend() { return std::suspend_always{}; }
        constexpr auto final_suspend() noexcept { return std::suspend_always{}; }
        constexpr void unhandled_exception() {}
    };

    struct iterator
    {
        std::coroutine_handle<promise_type> h;

        constexpr void operator++() { h.resume(); }
        constexpr T const& operator*() const { return *h.promise().value; }
        constexpr bool operator!=(std::default_sentinel_t) const { return !h.done(); }
    };


    constexpr Generator(std::coroutine_handle<promise_type> h)
        : _handle(h)
    {}

    constexpr ~Generator()
    {
        if (_handle) {
            _handle.destroy();
        } else {
            __builtin_trap();
        }
    }

    constexpr auto begin()
    {
        if (_handle) {
            _handle.resume();
        } else {
            __builtin_trap();
        }
        return iterator{_handle};
    }

    constexpr auto end() { return std::default_sentinel_t{}; }

    constexpr void forEach(auto&& func)
    {
        for (T const& val : *this) {
            func(val);
        }
    }

private:
    std::coroutine_handle<promise_type> _handle;
};


template<typename T>
inline auto range(T stop) -> Generator<T>
{
    if (stop > 0) {
        for (T i = 0; i != stop; i++) {
            co_yield i;
        }
    } else {
        for (T i = 0; i != stop; i--) {
            co_yield i;
        }
    }
}

template<typename T>
inline auto range(T start, T stop) -> Generator<T>
{
    if (start <= stop) {
        for (T i = start; i != stop; i++) {
            co_yield i;
        }
    } else {
        for (T i = stop; i != start; i--) {
            co_yield i;
        }
    }
}

template<typename T>
inline auto range(T start, T stop, T step) -> Generator<T>
{
    if (start <= stop) {
        for (T i = start; i >= start && i < stop; i += step) {
            co_yield i;
        }
    } else {
        for (T i = stop; i <= stop && i > start; i -= step) {
            co_yield i;
        }
    }
}

}  // namespace cm
