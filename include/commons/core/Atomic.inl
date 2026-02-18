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

enum class AtomicConstraint {

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

template<typename T, auto DefaultConstraint = AtomicConstraint::Strict>
struct Atomic
{
public:
    T load() const noexcept { return __atomic_load_n(&val_, 0); }

    void store(T val) const noexcept { __atomic_store(&val_, &val); }

    void clear() const noexcept
    {
        if constexpr (__is_same(T, bool)) {
            __atomic_clear(&val_, 0);
        } else {
            __atomic_store_n(&val_, 0, 0);
        }
    }

    void swap(Atomic& other) noexcept { __atomic_exchange(&other.val_, &this->val_, &this->val_); }

private:
    T val_;
};

template<auto Ordering = AtomicConstraint::Strict>
using AtomicBool = Atomic<bool, Ordering>;

template<typename T, auto Ordering = AtomicConstraint::Strict>
using AtomicPtr = Atomic<T*, Ordering>;

#endif
