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


///
/// Generates a string representation of an ArrayRef. Outputs (within square brackets) the string representation of each
/// element in the array, delimited by commas.
///
template<typename T>
constexpr void OutputString(ArrayRef<T> const& a, auto const& out)
{
    out('[');
    if (a.length() != 0) {
        usize i = 0;
        for (; i < a.length() - 1; i++) {
            auto const& value = a[i];
            OutputString(value, out);
            out(',');
            out(' ');
        }
        for (; i < a.length(); i++) {
            auto const& value = a[i];
            OutputString(value, out);
        }
    }
    out(']');
}

///
/// Generates a string representation of an Optional. If the Optional value is set, it outputs the string representation
/// of the value. Otherwise, outputs "None"
///
template<typename T>
requires (!__is_same(T, NoneType))
constexpr void Optional<T>::outputString(Optional<T> const& self, auto const& out)
{
    if (self.hasValue()) {
        // auto const& value = self.value();
        // Box(value).outputString(value, out);
        out('g');
        out('o');
        out('o');
        out('d');
    } else {
        out('N');
        out('o');
        out('n');
        out('e');
    }
}

}  // namespace cm
#endif
