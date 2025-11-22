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


namespace cm::Charset {

/*
 https://en.cppreference.com/w/c/string/byte/isalpha
*/
constexpr int isalpha(int c) { return (65 <= c && c <= 90) || (97 <= c && c <= 122); }

/*
 In the default C locale, only space and horizontal tab are classified as blank.
*/
constexpr int isblank(int c) { return (c == 9) || (c == 32); }

/*
 https://en.cppreference.com/w/c/string/byte/iscntrl
 Checks if the given character is a control character, i.e. codes 0x00-0x1F and 0x7F.
 The behavior is undefined if the value of ch is not representable as unsigned char and is not equal
 to EOF.
*/
constexpr int iscntrl(int c) { return (0x00 <= c && c <= 0x1f) || (c == 0x7f); }

/*
 https://en.cppreference.com/w/c/string/byte/isdigit
*/
constexpr int isdigit(int c) { return (48 <= c && c <= 57); }

/*
 https://en.cppreference.com/w/c/string/byte/isalnum
*/
constexpr int isalnum(int c) { return isdigit(c) || isalpha(c); }

/*
 https://en.cppreference.com/w/c/string/byte/isgraph
 Checks if the given character has a graphical representation.
*/
constexpr int isgraph(int c) { return (c > 32) && (c != 127); }

/*
 https://en.cppreference.com/w/c/string/byte/isspace
*/
constexpr int isspace(int c) { return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v'; }

/*
 https://en.cppreference.com/w/c/string/byte/islower
*/
constexpr int islower(int c) { return (97 <= c && c <= 122); }

/*
 https://en.cppreference.com/w/c/string/byte/isupper
*/
constexpr int isupper(int c) { return (65 <= c && c <= 90); }

/*
 https://en.cppreference.com/w/c/string/byte/isprint
*/
constexpr int isprint(int c) { return isgraph(c) || isspace(c); }

/*
 https://en.cppreference.com/w/c/string/byte/ispunct
 Checks if the given character is a punctuation character in the current C locale. The default C
 locale classifies the characters !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~ as punctuation.
*/
constexpr int ispunct(int c)
{
    return (33 <= c && c <= 47) || (58 <= c && c <= 63) || (91 <= c && c <= 96) || (123 <= c && c <= 126);
}

/*
 https://en.cppreference.com/w/c/string/byte/isxdigit
 Checks if the given character is a hexadecimal numeric character (0123456789abcdefABCDEF) or is
 classified as a hexadecimal character.
*/
constexpr int isxdigit(int c) { return isdigit(c) || (65 <= c && c <= 70) || (97 <= c && c <= 102); }

/*
 https://en.cppreference.com/w/c/string/byte/tolower
*/
constexpr int tolower(int c)
{
    if (isupper(c))
        return c + 32;
    return c;
}

/*
 https://en.cppreference.com/w/cpp/string/byte/toupper
*/
constexpr int toupper(int c)
{
    if (islower(c))
        return c - 32;
    return c;
}

}  // namespace cm::Charset
