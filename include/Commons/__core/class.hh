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

#ifdef __inline_core_header__

namespace cm {

class String;

///
/// This is something akin to the Class object in Java-- a constant record of properties about a type.
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
    uint32_t sizeBytes;
    uint32_t isPrimitive : 1;
    uint32_t isSigned : 1;
    uint32_t isFloatingPoint : 1;
    uint32_t isInteger : 1;

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
    static consteval Class make(char const* name)
    {
        Class result{};
        result.name = name;
        result.sizeBytes = sizeof(T);
        result.isPrimitive = !IsClass<T>;
        result.isSigned = !IsClass<T> && !IsUnsignedInteger<T>;
        result.isFloatingPoint = IsFloatingPoint<T>;
        result.isInteger = IsInteger<T>;

        // .

        if constexpr (!TriviallyDestructible<T>) {
            result.destructor = [](void* ptr) -> void {
                reinterpret_cast<T*>(ptr)->~T();
            };
        }
        if constexpr (!TriviallyDefaultConstructible<T> && DefaultConstructible<T>) {
            result.defaultConstructor = [](void* ptr) -> void {
                new (ptr) T();
            };
        }
        if constexpr (!TriviallyCopyConstructible<T> && CopyConstructible<T>) {
            result.copyConstructor = [](void* dst, void const* src) -> void {
                new (dst) T(*reinterpret_cast<T const*>(src));
            };
        }
        return result;
    }


    template<unsigned... Idxs>
    struct _name_buffer
    {
        consteval _name_buffer(auto... args)
            : data{args...}
        {}
        char data[sizeof...(Idxs) + 1];
    };

    template<typename T>
    static consteval auto _get_name_buffer()
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
        _Pragma("clang diagnostic push") {}
        _Pragma("clang diagnostic ignored \"-Wunsafe-buffer-usage\"") {}
        auto _to_name_buffer = []<size_t... Idxs>(char const* str, IntegerSequence<size_t, Idxs...>) consteval {
            return _name_buffer<Idxs...>(str[Idxs]..., '\0');
        };

        return _to_name_buffer(
            (Ptr::findSubstring(function, prefix) + (sizeof(prefix) - 1)),
            MakeIntegerSequence<
                size_t, (Ptr::findSubstring(function, suffix) -
                         (Ptr::findSubstring(function, prefix) + (sizeof(prefix) - 1)))>{});
        _Pragma("clang diagnostic pop") {}
    }
} *ClassPtr, &ClassRef;


template<typename T>
struct ClassStorage
{
    constexpr static auto _name_buffer = Class::_get_name_buffer<T>();
    constexpr static char const* name = _name_buffer.data;
    constexpr static auto c = Class::make<T>(name);
};

template<typename T>
constexpr Class const& ClassOf = ClassStorage<T>::c;


}  // namespace cm
#endif
