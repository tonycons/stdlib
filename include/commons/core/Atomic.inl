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
#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else


namespace cm {

enum class Atomicity {

    // Implies no inter-thread ordering constraints.
    Relaxed = __ATOMIC_RELAXED,

    // Creates an inter-thread happens-before constraint from the release (or stronger) semantic
    // store to this acquire load. Can prevent hoisting of code to before the operation.
    Acquire = __ATOMIC_ACQUIRE,

    // Creates an inter-thread happens-before constraint to acquire (or stronger) semantic loads
    // that read from this release store. Can prevent sinking of code to after the operation.
    Release = __ATOMIC_RELEASE,

    // Combines the effects of both Acquire and Release.
    AcquireRelease = __ATOMIC_ACQ_REL,

    // Enforces total ordering with all other Seq operations.
    Strict = __ATOMIC_SEQ_CST,
};

template<typename T, Atomicity M = Atomicity::Strict>
struct Atomic
{
public:
    Atomic() = default;
    Atomic(T val)
        : _val(val)
    {}

    T load() const noexcept { return __atomic_load_n(&_val, static_cast<int>(M)); }
    void store(T val) const noexcept { __atomic_store_n(&_val, val, static_cast<int>(M)); }

    void clear() const noexcept
    {
        if constexpr (__is_same(T, bool)) {
            __atomic_clear(&_val, 0);
        } else {
            __atomic_store_n(&_val, 0, static_cast<int>(M));
        }
    }

    void swap(Atomic& other) noexcept { __atomic_exchange(&other._val, &_val, &_val); }

    ///
    /// Atomically replaces the value with the result of addition of arg to the old value and returns the value
    /// held previously.
    ///
    template<Atomicity M_ = M>
    auto add(T const& arg) requires (IsInteger<T>)
    {
        return __atomic_fetch_add(&_val, arg, static_cast<int>(M_));
    }

    ///
    /// Atomically replaces the value with the result of subtraction of arg from the old value and returns the value
    /// held previously.
    ///
    template<Atomicity M_ = M>
    auto fetchSub(T const& arg) requires (IsInteger<T>)
    {
        return __atomic_fetch_sub(&_val, arg, static_cast<int>(M_));
    }

    template<Atomicity M_ = M>
    static void fence()
    {
        __c11_atomic_thread_fence(static_cast<int>(M_));
    }

private:
    T volatile _val;
};

template<auto Ordering = Atomicity::Strict>
using AtomicBool = Atomic<bool, Ordering>;

template<typename T, auto Ordering = Atomicity::Strict>
using AtomicPtr = Atomic<T*, Ordering>;

}  // namespace cm

#endif
