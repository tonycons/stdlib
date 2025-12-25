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

class String;

///
/// This is something like the Class object in Java-- a constant record of properties about a type.
/// Generated at compile time, stored at runtime.
///
typedef const class Class {
public:
    using IDType = u64;

    constexpr static bool PRINT_CLASS_IDS = true;
    char const* name;
    CFunction<void(void*)> destructor;
    CFunction<void(void*)> defaultConstructor;
    CFunction<void(void*, void const*)> copyConstructor;
    CFunction<void(void*, void const*)> copyAssignOperator;
    CFunction<void(void*, void*)> moveConstructor;
    CFunction<void(void*, void*)> moveAssignOperator;
    u32 sizeBytes;
    u32 isPrimitive : 1;
    u32 isSigned : 1;
    u32 isFloatingPoint : 1;
    u32 isInteger : 1;

    [[no_unique_address]] struct IDProperty : ComputedProperty<Class>
    {
        constexpr operator IDType() const
        {
            usize result = usize(size_t(this));
            return static_cast<IDType>(result);
        }
        String* toString() const;
    } id;

public:
    /**

    */
    static Class const& fromID(IDType id);

    /*

    */
    constexpr bool operator==(Class const& c) const { return name == c.name; }

    /*

    */
    template<typename T>
    static consteval Class init(char const* name)
    {
        Class result{};
        result.name = name;
        result.sizeBytes = sizeof(T);
        result.isPrimitive = !IsClass<T>;
        result.isSigned = !IsClass<T> && !IsUnsignedInteger<T>;
        result.isFloatingPoint = IsFloatingPoint<T>;
        result.isInteger = IsInteger<T>;

        result.destructor = [](void* ptr) -> void {
            if constexpr (!TriviallyDestructible<T>) {
                reinterpret_cast<T*>(ptr)->~T();
            }
        };
        if constexpr (DefaultConstructible<T>) {
            result.defaultConstructor = [](void* ptr) -> void {
                new (ptr) T();
            };
        }
        if constexpr (CopyConstructible<T>) {
            result.copyConstructor = [](void* dst, void const* src) -> void {
                new (dst) T(*static_cast<T const*>(src));
            };
        }
        if constexpr (CopyAssignable<T>) {
            result.copyAssignOperator = [](void* left, void const* right) -> void {
                *(static_cast<T*>(left)) = (*static_cast<T const*>(right));
            };
        }
        if constexpr (MoveConstructible<T>) {
            result.moveConstructor = [](void* left, void* right) -> void {
                new (left) T(static_cast<T&&>(*static_cast<T*>(right)));
            };
        }
        if constexpr (MoveAssignable<T>) {
            result.moveAssignOperator = [](void* left, void* right) -> void {
                *(static_cast<T*>(left)) = static_cast<T&&>(*static_cast<T*>(right));
            };
        }
        return result;
    }


    template<unsigned... Idxs>
    struct NameBuffer
    {
        consteval NameBuffer(auto... args)
            : data{args...}
        {}
        char data[sizeof...(Idxs) + 1];
    };

    template<typename T>
    static consteval auto _getNameBuffer()
    {
        // using namespace std::literals::string_view_literals;

#if defined(__clang__)
        constexpr static char prefix[6] = "[T = ";
        constexpr static char suffix[2] = "]";
        constexpr static auto function = __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
        constexpr static char prefix[10] = "with T = ";
        constexpr static char suffix[2] = "]";
        constexpr static auto function = __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
        constexpr static char prefix[17] = "type_name_array<";
        constexpr static char suffix[8] = ">(void)";
        constexpr static auto function = __FUNCSIG__;
#else
#error Unsupported compiler
#endif
        _Pragma("clang diagnostic push");
        _Pragma("clang diagnostic ignored \"-Wunsafe-buffer-usage\"");
        auto toNameBuffer = []<size_t... Idxs>(char const* str, IntegerSequence<size_t, Idxs...>) consteval {
            return NameBuffer<Idxs...>(str[Idxs]..., '\0');
        };

        return toNameBuffer(
            (Ptr::findSubstring(function, prefix) + (sizeof(prefix) - 1)),
            MakeIntegerSequence<
                size_t, (Ptr::findSubstring(function, suffix) -
                         (Ptr::findSubstring(function, prefix) + (sizeof(prefix) - 1)))>{});
        _Pragma("clang diagnostic pop");
    }
} *ClassPtr, &ClassRef;


template<typename T>
struct ClassData
{
    constexpr static auto nameBuffer = Class::_getNameBuffer<T>();
    constexpr static char const* name = nameBuffer.data;
    constexpr static auto class_ = Class::init<T>(name);
};

template<typename T>
constexpr Class const& ClassOf = ClassData<T>::class_;


}  // namespace cm
#endif
