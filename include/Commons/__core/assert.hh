// /*
//    Copyright 2025 Anthony A. Constantinescu.

// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.
// */

#pragma once
#ifdef __inline_core_header__

namespace cm {

// assert messages

// clang-format off

constexpr auto
    ASMS_DATA_CORRUPTION  = "Data corruption",
    ASMS_DATA_CORRUPTION2 = "Data corruption (Invalid checksum)",
    ASMS_BUG              = "Bug",
    ASMS_NO_MEMORY        = "Out of memory",
    ASMS_BAD_CIRCUMSTANCE = "Function called under the wrong circumstances",
    ASMS_PARAMETER        = "Invalid parameter",
    ASMS_BOUNDS           = "Index out of bounds";

#define ASMS_INVALID(NAME) "Invalid parameter \""#NAME"\""
// clang-format on

///
/// Stores the source code location of where the struct was instantiated.
/// Equivalent to std::source_location.
///
struct SourceLocation
{
    // NOLINTBEGIN
    decltype(__builtin_LINE()) line_;
    decltype(__builtin_COLUMN()) column_;
    decltype(__builtin_FILE()) file_;

#if defined(__clang__) || defined(__EDG__)
    decltype(__builtin_FUNCTION()) function_;
#else
    decltype(__builtin_FUNCSIG()) function_;
#endif
    // NOLINTEND
public:
    constexpr inline _noinline SourceLocation() noexcept = default;

    FORCEINLINE constexpr SourceLocation(unsigned line, unsigned column, char const* file) noexcept
        : line_(line), column_(column), file_(file), function_("")
    {}

    FORCEINLINE constexpr SourceLocation(
        unsigned line, unsigned column, char const* file, char const* function) noexcept
        : line_(line), column_(column), file_(file), function_(function)
    {}

    FORCEINLINE static consteval SourceLocation current(
        unsigned line = __builtin_LINE(), unsigned column = __builtin_COLUMN(), char const* file = __builtin_FILE(),
        char const* function = __builtin_FUNCTION()) noexcept
    {
        return SourceLocation{line, column, file, function};
    }

    FORCEINLINE constexpr auto line() const noexcept { return line_; }
    FORCEINLINE constexpr auto column() const noexcept { return column_; }
    FORCEINLINE constexpr auto file() const noexcept { return file_; }
    FORCEINLINE constexpr auto function() const noexcept { return function_; }
};

[[noreturn]]
void panic(char const* message, char const* reason, SourceLocation src);


constexpr void Assert(bool CONDITION, char const* message = "", SourceLocation src = SourceLocation::current())
{
    (void)message;
    if CONSTANT_EVALUATED {
        if (!(CONDITION))
            panic("Assertion failed", message, src);
    } else {
        if (!(CONDITION))
            panic("Assertion failed", message, src);
    }
}

}  // namespace cm

#endif
