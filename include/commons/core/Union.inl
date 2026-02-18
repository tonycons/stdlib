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
union UnionData;

// clang-format off
template<typename T0>
union UnionData<T0> {constexpr ~UnionData(){} T0 _0;};

template<typename T1, typename T2>
union UnionData<T1, T2> {constexpr ~UnionData(){} T1 _0;T2 _1; };

template<typename T1, typename T2, typename T3>
union UnionData<T1, T2, T3> {constexpr ~UnionData(){} T1 _0;T2 _1;T3 _2; };

template<typename T1, typename T2, typename T3, typename T4>
union UnionData<T1, T2, T3, T4> {constexpr ~UnionData(){} T1 _0;T2 _1;T3 _2;T4 _3;};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
union UnionData<T1, T2, T3, T4, T5> {constexpr ~UnionData(){} T1 _0;T2 _1;T3 _2;T4 _3;T5 _4;};

// clang-format on


template<typename... Types>
requires (sizeof...(Types) > 1 && sizeof...(Types) < 4)
class Union : decltype(impl::unionMetadata<Types...>()) {
private:
    using Metadata = decltype(impl::unionMetadata<Types...>());
    using Data = UnionData<Types...>;
    static consteval bool hasMetadata() { return sizeof(Metadata) > 1; }
    static consteval bool hasNonTrivialDestructor() { return !Metadata::triviallyDestructible; }
    static consteval bool hasNonTrivialCopyConstructor() { return !Metadata::triviallyCopyConstructible; }
    static consteval bool hasNonTrivialCopyAssignment() { return !Metadata::triviallyCopyAssignable; }
    static consteval bool hasNonTrivialMoveConstructor() { return !Metadata::triviallyMoveConstructible; }
    static consteval bool hasNonTrivialMoveAssignment() { return !Metadata::triviallyMoveAssignable; }

    UintRanged<sizeof...(Types)> _tag;
    UnionData<Types...> _data;

    template<typename T>
    constexpr static void _destroy(T& obj)
    {
        obj.~T();  // Inside the template, 'T' is known
    }

    /// Implements the strong matching rule. Read the Union doc to know what this does.
    template<typename From, typename To>
    constexpr static bool strongMatch()
    {
        return IsSame<From, To>
               // Two floating point numbers, where sizeof(To) >= sizeof(From)
               || (IsFloatingPoint<From> && IsFloatingPoint<To> && sizeof(To) >= sizeof(From))
               // Two Integers of the same sign, where sizeof(To) >= sizeof(From)
               ||
               (IsInteger<From> && IsInteger<To> && sizeof(To) >= sizeof(From) &&
                ((IsIntegerSigned<From> && IsIntegerSigned<To>) || (!IsIntegerSigned<From> && !IsIntegerSigned<To>)));
    }

    /// Implements the medium matching rule. Read the Union doc to know what this does.
    template<typename From, typename To>
    constexpr static bool mediumMatch()
    {
        return (IsFloatingPoint<From> && IsFloatingPoint<To>) || (IsInteger<From> && IsInteger<To>);
    }

    /// Implements the weak matching rule. Read the Union doc to know what this does.
    template<typename From, typename To>
    constexpr static bool weakMatch()
    {
        return ConvertibleTo<From, To>;
    }


    template<int I, typename V, typename... Types_>
    struct FindFirstStrongMatchTo;

    template<int I, typename V, typename... Types_>
    struct FindFirstMediumMatch;

    template<int I, typename V, typename... Types_>
    struct FindFirstWeakMatch;


    template<int I, typename V, typename Tn, typename... Rest>
    requires (strongMatch<V, Tn>())
    struct FindFirstStrongMatchTo<I, V, Tn, Rest...>
    {
        using Type = Tn;
        constexpr static auto Tag = I;
    };

    template<int I, typename V, typename Tn, typename... Rest>
    requires (mediumMatch<V, Tn>())
    struct FindFirstMediumMatch<I, V, Tn, Rest...>
    {
        using Type = Tn;
        constexpr static auto Tag = I;
    };


    template<int I, typename V, typename Tn, typename... Rest>
    requires (weakMatch<V, Tn>())
    struct FindFirstWeakMatch<I, V, Tn, Rest...>
    {
        using Type = Tn;
        constexpr static auto Tag = I;
    };

    //

    template<int I, typename V, typename Tn, typename... Rest>
    requires (!strongMatch<V, Tn>())
    struct FindFirstStrongMatchTo<I, V, Tn, Rest...>
    {
        using Type = FindFirstStrongMatchTo<I + 1, V, Rest...>::Type;
        constexpr static auto Tag = FindFirstStrongMatchTo<I + 1, V, Rest...>::Tag;
    };

    template<int I, typename V, typename Tn, typename... Rest>
    requires (!mediumMatch<V, Tn>())
    struct FindFirstMediumMatch<I, V, Tn, Rest...>
    {
        using Type = FindFirstMediumMatch<I + 1, V, Rest...>::Type;
        constexpr static auto Tag = FindFirstMediumMatch<I + 1, V, Rest...>::Tag;
    };


    template<int I, typename V, typename Tn, typename... Rest>
    requires (!weakMatch<V, Tn>())
    struct FindFirstWeakMatch<I, V, Tn, Rest...>
    {
        using Type = FindFirstWeakMatch<I + 1, V, Rest...>::Type;
        constexpr static auto Tag = FindFirstWeakMatch<I + 1, V, Rest...>::Tag;
    };

    template<int I, typename V>
    struct FindFirstStrongMatchTo<I, V>
    {
        using Type = void;
        constexpr static auto Tag = -1;
    };

    template<int I, typename V>
    struct FindFirstMediumMatch<I, V>
    {
        using Type = void;
        constexpr static auto Tag = -1;
    };

    template<int I, typename V>
    struct FindFirstWeakMatch<I, V>
    {
        using Type = void;
        constexpr static auto Tag = -1;
    };
    //
    template<typename V>
    using MatchTo = decltype([] consteval {
        using SM = FindFirstStrongMatchTo<0, V, Types...>;
        using MM = FindFirstMediumMatch<0, V, Types...>;
        using WM = FindFirstWeakMatch<0, V, Types...>;
        if constexpr (not IsSame<typename SM::Type, void>) {
            return SM{};
        } else if constexpr (not IsSame<typename MM::Type, void>) {
            return MM{};
        } else {
            return WM{};
        }
    }());

public:
    ///
    /// There is no default constructor because the union must have an active type
    ///
    Union() = delete;

    ///
    /// Destructor
    ///
    constexpr ~Union()
    {
        if constexpr (sizeof...(Types) < 2) {
            _destroy(_data._0);
        } else if constexpr (sizeof...(Types) >= 2) {
            if (_tag == 0) {
                _destroy(_data._0);
                return;
            }
            if (_tag == 1) {
                _destroy(_data._1);
                return;
            }
        }
        if constexpr (sizeof...(Types) >= 3) {
            if (_tag == 2) {
                _destroy(_data._2);
                return;
            }
        }
        if constexpr (sizeof...(Types) >= 4) {
            if (_tag == 3) {
                _destroy(_data._3);
                return;
            }
        }
    }


    template<typename V>
    requires (!IsSame<V, Union>)
    constexpr Union(V&& value)
        : _tag(MatchTo<V>::Tag), _data([&] constexpr {
              constexpr auto Tag = MatchTo<V>::Tag;
              if constexpr (Tag == 0) {
                  return Data{._0 = Forward<V>(value)};
              } else if constexpr (Tag == 1) {
                  return Data{._1 = Forward<V>(value)};
              } else if constexpr (Tag == 2) {
                  return Data{._2 = Forward<V>(value)};
              } else if constexpr (Tag == 3) {
                  return Data{._3 = Forward<V>(value)};
              } else if constexpr (Tag == 4) {
                  return Data{._4 = Forward<V>(value)};
              } else {
                  static_assert(false, "None of the union types can be initialized from the provided value");
              }
          }())
    {}

    template<typename V>
    requires (!IsSame<typename MatchTo<V>::Type, void>)
    constexpr Union& operator=(V&& value)
    {
        constexpr auto Tag = MatchTo<V>::Tag;
        if (_tag != Tag) {
            this->~Union();
            new (this) Union(Forward<V>(value));
        } else {
            ref<MatchTo<V>::Type>() = Forward<V>(value);
        }
        _tag = Tag;
        return *this;
    }

    ///
    /// Copy constructor
    /// @param other Another union of the same type
    ///
    constexpr Union(Union const& other)
        : _tag(other._tag), _data([&] constexpr {
              if (other._tag == 0) {
                  return Data{._0 = other._data._0};
              }
              if (other._tag == 1) {
                  return Data{._1 = other._data._1};
              }
              if constexpr (sizeof...(Types) > 2) {
                  if (other._tag == 2)
                      return Data{._2 = other._data._2};
              }
              if constexpr (sizeof...(Types) > 3) {
                  if (other._tag == 3)
                      return Data{._3 = other._data._3};
              }
              static_assert(sizeof...(Types) < 5);
              return Data{._0 = other._data._0};
          }())
    {}

    ///
    /// Copy assignment operator
    /// @param other .
    /// @return .
    constexpr Union& operator=(Union const& other)
    {
        if (_tag != other._tag) {
            this->~Union();
            new (this) Union(other);
            return *this;
        }
        if (_tag == 0) {
            _data._0 = other._data._0;
        }
        if (_tag == 1) {
            _data._1 = other._data._1;
        }
        if constexpr (sizeof...(Types) > 2) {
            if (_tag == 2)
                _data._2 = other._data._2;
        }
        if constexpr (sizeof...(Types) > 3) {
            if (_tag == 3)
                _data._3 = other._data._3;
        }
        return *this;
    }

    ///
    /// Returns true if the active type in the Union is the given type T
    ///
    template<IsSameAsOneOf<Types...> T>
    [[nodiscard]] constexpr bool is() const noexcept
    {
        return _tag == MatchTo<T>::Tag;
    }

    ///
    /// Returns a reference to the active type of the union as type T, expects you to have already checked that it is
    /// type T.
    ///
    template<IsSameAsOneOf<Types...> T>
    [[nodiscard]] constexpr auto& ref(this auto&& self) noexcept
    {
        Assert(self.template is<T>());
        constexpr auto Tag = MatchTo<T>::Tag;
        if constexpr (Tag == 0) {
            return self._data._0;
        } else if constexpr (Tag == 1) {
            return self._data._1;
        } else if constexpr (Tag == 2) {
            return self._data._2;
        } else if constexpr (Tag == 3) {
            return self._data._3;
        } else if constexpr (Tag == 4) {
            return self._data._4;
        } else {
            static_assert(false, "[internal issue] invalid tag value");
        }
    }

    ///
    /// Returns copy of the active type of the union as type T, expects you to have already checked that it is
    /// type T.
    ///
    template<IsSameAsOneOf<Types...> T>
    [[nodiscard]] constexpr auto val() const noexcept
    {
        return ref<T>();
    }


    ///
    /// Returns a reference to the active type of the union as type T, otherwise, if the active type is not T,
    /// returns a default value.
    ///
    template<typename T>
    constexpr auto& getOrDefault(this auto&& self, auto& defaultValue) noexcept
    {
        if (self.template is<T>()) {
            return self.template ref<T>();
        } else {
            return defaultValue;
        }
    }

    ///
    /// Returns the tag value.
    ///
    constexpr auto tag() const noexcept { return _tag; }

    ///
    /// Performs matching on the union's active type, like the Rust match clause.
    /// For each provided closure, the closure that will be called is the one where the type of its first argument
    /// matches the type stored in the union.
    ///
    auto match(auto func, auto... funcs) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>() || this->is<RefRemoved<T>>()) {
            return func(this->template ref<T>());
        }
        return this->match(funcs...);
    }

    auto match(auto func) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>()) {
            return func(this->template ref<T>());
        }
        __builtin_trap();
    }

    template<auto Default>
    auto matchOr(auto func, auto... funcs) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>()) {
            return func(this->template ref<T>());
        }
        return this->template matchOr<Default>(funcs...);
    }

    template<auto Default>
    auto matchOr(auto func) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>()) {
            return func(this->template ref<T>());
        }
        if constexpr (IsFunction<decltype(Default)> || IsClass<decltype(Default)>) {
            return Default();
        } else {
            return Default;
        }
    }
};

}  // namespace cm

#endif
