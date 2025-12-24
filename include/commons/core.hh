/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/core.hh
   Description: Exports the core features of this library
*/

#pragma once
#define __inline_core_header__

#include "core/base_definitions.hh"     // IWYU pragma: keep
#include "core/cstring.hh"              // IWYU pragma: keep
#include "core/range.hh"                // IWYU pragma: keep
#include "core/reflection_type.hh"      // IWYU pragma: keep
#include "core/comparable.hh"           // IWYU pragma: keep
#include "core/generator.hh"            // IWYU pragma: keep
#include "core/initializer_list.hh"     // IWYU pragma: keep
#include "core/tuple.hh"                // IWYU pragma: keep
#include "core/reflection_function.hh"  // IWYU pragma: keep
#include "core/function.hh"             // IWYU pragma: keep
#include "core/intbase.hh"              // IWYU pragma: keep
#include "core/assert.hh"               // IWYU pragma: keep
#include "core/union.hh"                // IWYU pragma: keep
#include "core/result.hh"               // IWYU pragma: keep
#include "core/optional.hh"             // IWYU pragma: keep
#include "core/errors.hh"               // IWYU pragma: keep
#include "core/profiler.hh"             // IWYU pragma: keep


#include "core/array_iterator.hh"  // IWYU pragma: keep
#include "core/index.hh"           // IWYU pragma: keep
#include "core/iterable.hh"        // IWYU pragma: keep
#include "core/arrayref.hh"        // IWYU pragma: keep
#include "core/string_ref.hh"      // IWYU pragma: keep
#include "core/math_int.hh"        // IWYU pragma: keep
#include "core/math_float.hh"      // IWYU pragma: keep
#include "core/pointer.hh"         // IWYU pragma: keep

constexpr void ::cm::__outputString(auto const& value, auto const& out)
{
    constexpr IntBaseFmt Base = IntBaseFmt::B10;
    constexpr IntegerParsingScheme S = IntegerParsingScheme::DEFAULT;
    using T = CVRefRemoved<decltype(value)>;

    if constexpr (IsUnderlyingTypeOneOf<T, char, char8_t>) {
        out(value);
    } else if constexpr (IsBool<T>) {
        if (value) {
            out('t');
            out('r');
            out('u');
            out('e');
        } else {
            out('f');
            out('a');
            out('l');
            out('s');
            out('e');
        }
    } else if constexpr (IsInteger<T>) {
        if constexpr (S == IntegerParsingScheme::JSON) {
            static_assert(Base == IntBaseFmt::B10, "JSON only supports base 10");
        }
        auto num = value;
        if constexpr (IsIntegerSigned<T>) {
            if (num < 0) {
                out('-');
                num = static_cast<T>(-value);
            }
        }
        // Handle prefixes
        if constexpr (Base == IntBaseFmt::B2) {
            if constexpr (S == IntegerParsingScheme::YAML) {
                out("0b");
            }
        } else if constexpr (Base == IntBaseFmt::B8) {
            if constexpr (S == IntegerParsingScheme::YAML) {
                out("0o");
            }
        } else if constexpr (Base == IntBaseFmt::B16) {
            if constexpr (S == IntegerParsingScheme::YAML) {
                out("0x");
            }
        }
        if (num == 0) {
            out(intBaseTables[uint(Base)].charSet[0]);
            return;
        }
        do {
            out(intBaseTables[uint(Base)].charSet[msd<uint(Base)>(num)]);
            num /= uint(Base);
        } while (num != 0);

    } else if constexpr (IsFloatingPoint<T>) {

        // double or float
        auto val = double(value);
        i64 integer;
        int zero_threshold = 16;
        int zero_seq_len = 0;

        if (__builtin_isnan(val)) {
            out('N'), out('a'), out('N');
            return;
        } else if (__builtin_isinf(val)) {
            if (__builtin_signbit(val)) {
                out('-'), out('i'), out('n'), out('f');
                return;
            } else {
                out('i'), out('n'), out('f');
                return;
            }
        }
        if (val < 0) {
            out('-');
            val = -val;
        }
        integer = i64(val);
        if (integer == 0) {
            out('0');
            return;
        }
        OutputString(integer, out);
        out('.');

        while (true) {
            val = val - static_cast<double>(i64(val));
            val *= 10;
            integer = i64(val);
            if (integer == 0) {
                zero_seq_len++;
                out('0');
                if (zero_seq_len >= zero_threshold) {
                    return;
                }
            } else {
                zero_seq_len = 0;
                OutputString(integer, out);
            }
            zero_threshold--;
        }
    }
}


#include "core/property.hh"  // IWYU pragma: keep
#include "core/rng.hh"       // IWYU pragma: keep
#include "core/hash.hh"      // IWYU pragma: keep
#include "core/class.hh"     // IWYU pragma: keep

namespace cm {}  // namespace cm

#undef __inline_core_header__
