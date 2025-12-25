/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: core/union
   Description:
    Implements a Union type that can hold one of several types at a time.
    The Union type uses a strong, medium, and weak matching rule to determine which type to initialize the union with,
    based on the type of the value passed to the constructor.

    The functionality is similar to that of Rust Enum.
    It follows the exact same memory layout to the Rust Enums-
    Essentially "struct { u8 tag; union { .... } u; };"
    Except while Rust uses a 16-bit integer as a tag, this can use a byte as the tag if there are less than 255 types
    See https://patshaughnessy.net/2018/3/15/how-rust-implements-tagged-unions
*/

#pragma once
#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else

namespace cm {

namespace impl {

template<bool>
class UnionDestructorCallback {};

template<>
class UnionDestructorCallback<true> {
protected:
    CFunction<void(void*)> _dtor;
};

template<bool>
class UnionCopyConstructorCallback {};

template<>
class UnionCopyConstructorCallback<true> {
protected:
    CFunction<void(void*, void const*)> _copyCtor;
};

template<bool>
class UnionCopyAssignmentCallback {};

template<>
class UnionCopyAssignmentCallback<true> {
protected:
    CFunction<void(void*, void const*)> _copyAssignOp;
};

template<bool>
class UnionMoveConstructorCallback {};

template<>
class UnionMoveConstructorCallback<true> {
protected:
    CFunction<void(void*, void const*)> _moveCtor;
};

template<bool>
class UnionMoveAssignmentCallback {};

template<>
class UnionMoveAssignmentCallback<true> {
protected:
    CFunction<void(void*, void const*)> _moveAssignOp;
};

template<typename... Types>
struct UnionExtraVariables
{
    constexpr static bool triviallyDestructible = ((TriviallyDestructible<Types>) && ...);
    constexpr static bool triviallyCopyConstructible = ((TriviallyCopyConstructible<Types>) && ...);
    constexpr static bool triviallyCopyAssignable = ((TriviallyCopyAssignable<Types>) && ...);
    constexpr static bool triviallyMoveConstructible = ((TriviallyMoveConstructible<Types>) && ...);
    constexpr static bool triviallyMoveAssignable = ((TriviallyMoveAssignable<Types>) && ...);
};

/// Defines a base class for the union which has the following purpose:
/// If any of the types in the union are not trivially destructible/trivially copy constructible/trivially assignable,
/// then the class defines a pointer to the destructor/copy constructor/assignment operator of the current type in the
/// union. otherwise, the class is empty. this is an optimization allowing the union to omit these function pointers if
/// it is not necessary.
template<typename... Types>
consteval auto unionMetadata()
{
    using S = UnionExtraVariables<Types...>;

    if constexpr (
        S::triviallyDestructible && S::triviallyCopyConstructible && S::triviallyCopyAssignable &&
        S::triviallyMoveConstructible && S::triviallyMoveAssignable)
    {
        constexpr static S s;
        return s;
    } else {
        struct R : S
        {
            Class const* _class = nullptr;
        } static constexpr r;
        return r;
    }
}


}  // namespace impl


template<typename... Types>
class Union : decltype(impl::unionMetadata<Types...>()) {
public:
    using Metadata = decltype(impl::unionMetadata<Types...>());
    static consteval bool hasMetadata() { return sizeof(Metadata) > 1; }
    static consteval bool hasNonTrivialDestructor() { return !Metadata::triviallyDestructible; }
    static consteval bool hasNonTrivialCopyConstructor() { return !Metadata::triviallyCopyConstructible; }
    static consteval bool hasNonTrivialCopyAssignment() { return !Metadata::triviallyCopyAssignable; }
    static consteval bool hasNonTrivialMoveConstructor() { return !Metadata::triviallyMoveConstructible; }
    static consteval bool hasNonTrivialMoveAssignment() { return !Metadata::triviallyMoveAssignable; }

    /// Used to keep track of the type that was chosen to be initialized in the Union constructor.
    template<typename T, int I>
    struct InitSuccess
    {
        using WhichOne = T;
        constexpr static auto Tag = I;
    };
    struct InitFailure
    {};

private:
    // the Nth type stored in the union
    UintRanged<sizeof...(Types)> _tag;
    alignas(max(alignof(Types)...)) u8 _data[max(sizeof(Types)...)];

    /// Implements the strong matching rule. Read the Union doc to know what this does.
    struct Strong
    {
        template<typename From, typename To>
        constexpr static bool match()
        {
            return IsSame<From, To>
                   // Two floating point numbers, where sizeof(To) >= sizeof(From)
                   || (IsFloatingPoint<From> && IsFloatingPoint<To> && sizeof(To) >= sizeof(From))
                   // Two Integers of the same sign, where sizeof(To) >= sizeof(From)
                   || (IsInteger<From> && IsInteger<To> && sizeof(To) >= sizeof(From) &&
                       ((IsIntegerSigned<From> && IsIntegerSigned<To>) ||
                        (!IsIntegerSigned<From> && !IsIntegerSigned<To>)));
        }
    };

    /// Implements the medium matching rule. Read the Union doc to know what this does.
    struct Medium
    {
        template<typename From, typename To>
        constexpr static bool match()
        {
            return (IsFloatingPoint<From> && IsFloatingPoint<To>) || (IsInteger<From> && IsInteger<To>);
        }
    };

    /// Implements the weak matching rule. Read the Union doc to know what this does.
    struct Weak
    {
        template<typename From, typename To>
        constexpr static bool match()
        {
            return ConvertibleTo<From, To>;
        }
    };

    template<typename matchFn, int I, typename U, typename... Tn>
    struct Initializer;

    template<typename matchFn, int I, typename From, typename T1, typename... Tn>
    struct Initializer<matchFn, I, From, T1, Tn...>
    {
        template<typename V>
        constexpr static auto match(void* data, V&& value)
        {
            static_assert(IsSame<CVRefRemoved<V>, CVRefRemoved<From>>);
            if constexpr (matchFn::template match<From, T1>()) {
                new (data) T1(Forward<V>(value));
                return InitSuccess<T1, I>{};
            } else {
                return Initializer<matchFn, I + 1, From, Tn...>::match(data, Forward<V>(value));
            }
        }
    };

    template<typename matchFn, int I, typename From, typename To>
    struct Initializer<matchFn, I, From, To>
    {
        template<typename V>
        constexpr static auto match(void* data, V&& value)
        {
            static_assert(IsSame<CVRefRemoved<V>, CVRefRemoved<From>>);
            if constexpr (matchFn::template match<From, To>()) {
                _Pragma("GCC diagnostic push");
                _Pragma("GCC diagnostic ignored \"-Wsign-conversion\"");
                new (data) To(Forward<V>(value));
                _Pragma("GCC diagnostic pop");
                return InitSuccess<To, I>{};
            } else {
                return InitFailure{};
            }
        }
    };

    template<typename matchFn, int I, typename U>
    struct Initializer<matchFn, I, U>
    {
        template<typename V>
        constexpr static auto match(void*, V&&)
        {
            return InitFailure{};
        }
    };

    /**
     * Attempts to initialize the union value of type T1 with a given value. If the union value of type T1 cannot be
     * initialized by the given value, then attempts to initialize the union value with the next type in Tn.
     */
    template<int I, typename U, typename... Tn>
    struct TryInit
    {
        template<typename V>
        constexpr static auto next(void* data, V&& value)
        {
            // attemps to initialize the union with the first type that is a strong match to value.
            if constexpr (IsSame<
                              decltype(Initializer<Strong, I, U, Tn...>::match(data, Forward<V>(value))), InitFailure>)
            {
                // otherwise, the first type that is a medium match to value.
                if constexpr (IsSame<
                                  decltype(Initializer<Medium, I, U, Tn...>::match(data, Forward<V>(value))),
                                  InitFailure>)
                {
                    // otherwise, the first type that is a weak match to value.
                    return Initializer<Weak, I, U, Tn...>::match(data, Forward<V>(value));
                } else {
                    return Initializer<Medium, I, U, Tn...>::match(data, Forward<V>(value));
                }
            } else {
                return Initializer<Strong, I, U, Tn...>::match(data, Forward<V>(value));
            }
        }
    };

    template<typename T>
    constexpr inline void storeCallbacksFor()
    {
        if constexpr (hasMetadata()) {
            Metadata::_class = &ClassOf<T>;
        }
    }

public:
    ///
    /// Destructor for the union. If all types are trivially destructible, it is also trivial. Otherwise, it
    /// destroys whichever type is active in the union.
    ///
    constexpr ~Union() requires (!hasNonTrivialDestructor())
    = default;
    constexpr ~Union() requires (hasNonTrivialDestructor())
    {
        if (Metadata::_class) {
            Metadata::_class->destructor(_data);
        }
    }

    ///
    /// Copy constructor
    ///
    constexpr inline Union(Union const& other) requires (!hasNonTrivialCopyConstructor())
    = default;
    constexpr inline Union(Union const& other) requires (hasNonTrivialCopyConstructor())
    {
        Metadata::_class = other._class;
        _tag = other._tag;
        Assert(Metadata::_class->copyConstructor, "Type in union not copy constructible");
        Metadata::_class->copyConstructor(_data, other._data);
    }

    ///
    /// Copy assignment operator
    ///
    constexpr inline Union& operator=(Union const& other)
        requires (!hasNonTrivialDestructor() && !hasNonTrivialCopyAssignment() && !hasNonTrivialCopyConstructor())
        = default;
    constexpr inline Union& operator=(Union const& other)
        requires (hasNonTrivialDestructor() || hasNonTrivialCopyAssignment() || hasNonTrivialCopyConstructor())
    {
        if (_tag == other._tag) {  // if the type stored in this is the same type stored in other, call its copy
                                   // assignment operator.
            if constexpr (hasNonTrivialCopyAssignment()) {
                Assert(Metadata::_class->copyAssignOperator, "Type in union not copy assignable");
                Metadata::_class->copyAssignOperator(_data, other._data);
            } else {
                __builtin_memcpy_inline(_data, other._data, sizeof(_data));
            }
        } else {
            // otherwise, if other stores a different type, destroy the current type and call the copy constructor
            if constexpr (hasNonTrivialDestructor()) {
                Metadata::_class->destructor(_data);
            }
            Metadata::_class = other._class;
            if constexpr (hasNonTrivialCopyConstructor())  //
            {
                Assert(Metadata::_class->copyConstructor, "Type in union not copy constructible");
                Metadata::_class->copyConstructor(_data, other._data);
            } else {
                __builtin_memcpy_inline(_data, other._data, sizeof(_data));
            }
            _tag = other._tag;
        }
        return *this;
    }

    ///
    /// Move constructor
    ///
    constexpr inline Union(Union&& other) requires (!hasNonTrivialMoveConstructor())
    = default;
    constexpr inline Union(Union&& other) requires (hasNonTrivialMoveConstructor())
    {
        Metadata::_class = other._class;
        _tag = other._tag;
        Assert(Metadata::_class->moveConstructor, "Type in union not move constructible");
        Metadata::_class->moveConstructor(_data, other._data);
    }

    ///
    /// Move assignment operator
    ///
    constexpr inline Union& operator=(Union&& other)
        requires (!hasNonTrivialDestructor() && !hasNonTrivialMoveAssignment() && !hasNonTrivialMoveConstructor())
        = default;
    constexpr inline Union& operator=(Union&& other)
        requires (hasNonTrivialDestructor() || hasNonTrivialMoveAssignment() || hasNonTrivialMoveConstructor())
    {
        if (_tag == other._tag) {  // if the type stored in this is the same type stored in other, call its copy
                                   // assignment operator.
            if constexpr (hasNonTrivialMoveAssignment()) {
                Assert(Metadata::_class->moveAssignOperator, "Type in union not move assignable");
                Metadata::_class->moveAssignOperator(_data, other._data);
            } else {
                __builtin_memcpy_inline(_data, other._data, sizeof(_data));
            }
        } else {  // otherwise, if other stores a different type, destroy the current type and call the copy constructor
            if constexpr (hasNonTrivialDestructor()) {
                Metadata::_class->destructor(_data);
            }
            Metadata::_class = other._class;
            if constexpr (hasNonTrivialMoveConstructor()) {
                Assert(Metadata::_class->moveConstructor, "Type in union not move constructible");
                Metadata::_class->moveConstructor(_data, other._data);
            } else {
                __builtin_memcpy_inline(_data, other._data, sizeof(_data));
            }
            _tag = other._tag;
        }
        if constexpr (hasNonTrivialDestructor()) {
            other._class = nullptr;
        }
        return *this;
    }

    ///
    /// Constructs the union from a specific type.
    ///
    template<typename GivenType>
    constexpr Union(GivenType&& t1)
        // "None of the union types can be initialized by the given type."
        requires (!IsSame<decltype(TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1))), InitFailure>)
    {
        using Init = decltype(TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1)));
        TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1));
        _tag = static_cast<unsigned char>(Init::Tag);
        storeCallbacksFor<typename Init::WhichOne>();
    }

    ///
    /// Changes the type currently in the union.
    ///
    template<typename GivenType>
    constexpr Union& operator=(GivenType&& t1)
    {
        using Init = decltype(TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1)));
        auto newTag = static_cast<decltype(_tag)>(Init::Tag);

        if constexpr (hasNonTrivialDestructor()) {
            Metadata::_dtor(_data);
        }
        TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1));
        storeCallbacksFor<typename Init::WhichOne>();
        _tag = newTag;
        return *this;
    }

    ///
    /// Returns true if the active type in the Union is the given type T
    ///
    template<typename T>
    constexpr inline bool is() const noexcept
    {
        if constexpr (CopyConstructible<T>) {
            auto const& k = (_ref<T>());
            using Init = decltype(TryInit<0, T, Types...>::next(const_cast<u8*>(_data), k));
            if constexpr (IsSame<Init, InitFailure>) {
                return false;
            } else {
                return _tag == Init::Tag;
            }
        } else if constexpr (MoveConstructible<T>) {
            using Init = decltype(TryInit<0, T, Types...>::template next<T&&>(
                const_cast<u8*>(_data), static_cast<T&&>(const_cast<T&>(_ref<T>()))));
            if constexpr (IsSame<Init, InitFailure>) {
                return false;
            } else {
                return _tag == Init::Tag;
            }
        } else {
            using Init = decltype(TryInit<0, T, Types...>::next(const_cast<u8*>(_data), (T(_ref<T>()))));
            if constexpr (IsSame<Init, InitFailure>) {
                return false;
            } else {
                return _tag == Init::Tag;
            }
        }
    }

    ///
    /// Unsafe-- effectively returns value of reinterpret_cast<T*>(union.data);
    ///
    template<typename T>
    NODISCARD inline T const& _ref() const noexcept
    {
        if constexpr (IsReference<T>) {
            using U = RefRemoved<T>;
            T const& ref = **reinterpret_cast<U const* const*>(_data);
            return ref;
        } else {
            T const& ref = *reinterpret_cast<T const*>(_data);
            return ref;
        }
    }
    template<typename T>
    NODISCARD inline T& _ref() noexcept
    {
        return const_cast<T&>(static_cast<Union const*>(this)->_ref<T>());
    }

    ///
    /// Gets the active type of the union as type T, expects you to have already checked that it is type T.
    ///
    template<typename T>
    NODISCARD inline T const& get() const noexcept
    {
        Assert(is<T>());
        return const_cast<ConstRemoved<decltype(_ref<T>())>>(_ref<T>());
    }
    template<typename T>
    NODISCARD inline T& get() noexcept
    {
        return const_cast<T&>(static_cast<Union const*>(this)->get<T>());
    }

    ///
    /// Gets the active type of the union as type T, otherwise, if the active type is not T, returns a default
    /// value.
    ///
    template<typename T>
    NODISCARD inline T const& getOrDefault(T const& defaultValue) const noexcept
    {
        if (is<T>()) {
            return const_cast<ConstRemoved<decltype(_ref<T>())>>(_ref<T>());
        } else {
            return defaultValue;
        }
    }

    ///
    /// Returns the tag value.
    ///
    NODISCARD inline auto tag() const noexcept { return _tag; }

    ///
    /// Performs matching on the union's active type, like the Rust match clause.
    /// For each provided closure, the closure that will be called is the one where the type of its first argument
    /// matches the type stored in the union
    ///
    inline auto match(auto func, auto... funcs) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>() || this->is<RefRemoved<T>>()) {
            return func(this->get<T>());
        } else {
            return this->match(funcs...);
        }
    }

    inline auto match(auto func) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>()) {
            return func(this->get<T>());
        } else {
            __builtin_trap();
            __builtin_unreachable();
        }
    }

    template<auto Default>
    inline auto matchOr(auto func, auto... funcs) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>()) {
            return func(this->get<T>());
        } else {
            return this->matchOr<Default>(funcs...);
        }
    }

    template<auto Default>
    inline auto matchOr(auto func) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>()) {
            return func(this->get<T>());
        } else {
            if constexpr (IsFunction<decltype(Default)> || IsClass<decltype(Default)>) {
                return Default();
            } else {
                return Default;
            }
        }
    }
};

}  // namespace cm
#endif
