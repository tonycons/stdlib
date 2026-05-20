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
#include <commons/resources/OneCharStringTable.hh>


namespace cm {

/*
 https://en.cppreference.com/w/c/string/byte/isspace
*/
constexpr bool charIsSpace(char c)
{
    // Reference: https://en.wikipedia.org/wiki/Whitespace_character
    switch (c) {
    case ' ':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case '\v': return true;
    default: return false;
    }
}

constexpr bool charIsSpace(wchar_t c)
{
    // Reference: https://en.wikipedia.org/wiki/Whitespace_character
    switch (c) {
    case 0:
    case L' ':
    case L'\f':
    case L'\n':
    case L'\r':
    case L'\t':
    case L'\v':
    case L'\u0085':                // next line
    case L'\u00A0':                // no-break space
    case L'\u1680':                // ogham space mark
    case L'\u2000' ... L'\u200A':  // custom width spaces
    case L'\u2028' ... L'\u2029':  //
    case L'\u202F':                // narrow no-break space
    case L'\u205F':                // medium mathematical space
    case L'\u3000':                // ideographic space
        return true;
    default: return false;
    }
}


static_assert([]() -> bool {
    for (wchar_t c : L"	\f\n\r\t\v\u0085\u00A0\u1680\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200A"
                     L"\u2028\u2029\u202F\u205F\u3000")
    {
        if (!charIsSpace(c))
            return false;
    }
    return true;
}());


/*
constexpr char const* asciiNormalize(wchar_t c)
{
    switch (c) {
    case 0 ... 128: return Data::oneCharStringTable(static_cast<char>(c));

    ///// SPACE ' ' similar
    case L'\u00A0':                // no-break space
    case L'\u1680':                // ogham space mark
    case L'\u2000' ... L'\u200A':  // custom width spaces
    case L'\u202F':                // narrow no-break space
    case L'\u205F':                // medium mathematical space
    case L'\u3000':                // ideographic space
        return " ";

    ///// '\n' similar
    case L'\u0085':
    case L'\u2028' ... L'\u2029': return "\n";

    // fractions
    case 0x2150 ... 0x215f:
        return (char const*[]){"1/7", "1/9", "1/10", "1/3", "2/3", "1/5", "2/5", "3/5",
                               "4/5", "1/6", "5/6",  "1/8", "3/8", "5/8", "7/8", "1/"}[c & 0xf];
    //// Roman numerals
    case 0x2160 ... 0x216f:
        return (char const*[]){"I",  "II", "III", "IV",  "V", "VI", "VII", "VIII",
                               "IX", "X",  "XI",  "XII", "L", "C",  "D",   "M"}[c & 0xf];
    case 0x2170 ... 0x217f:
        return (char const*[]){"i",  "ii", "iii", "iv",  "v", "vi", "vii", "viii",
                               "ix", "x",  "xi",  "xii", "l", "c",  "d",   "m"}[c & 0xf];
    // Enclosed Alphanumerics
    case 0x2460 ... 0x24ff:
        return (char const*[]){
            "(1)",  "(2)",  "(3)",  "(4)",  "(5)",  "(6)",  "(7)",  "(8)",  "(9)",  "(10)", "(11)", "(12)", "(13)",
            "(14)", "(15)", "(16)", "(17)", "(18)", "(19)", "(20)", "(1)",  "(2)",  "(3)",  "(4)",  "(5)",  "(6)",
            "(7)",  "(8)",  "(9)",  "(10)", "(11)", "(12)", "(13)", "(14)", "(15)", "(16)", "(17)", "(18)", "(19)",
            "(20)", "1.",   "2.",   "3.",   "4.",   "5.",   "6.",   "7.",   "8.",   "9.",   "10.",  "11.",  "12.",
            "13.",  "14.",  "15.",  "16.",  "17.",  "18.",  "19.",  "20.",  "(a)",  "(b)",  "(c)",  "(d)",  "(e)",
            "(f)",  "(g)",  "(h)",  "(i)",  "(j)",  "(k)",  "(l)",  "(m)",  "(n)",  "(o)",  "(p)",  "(q)",  "(r)",
            "(s)",  "(t)",  "(u)",  "(v)",  "(w)",  "(x)",  "(y)",  "(z)",  "(A)",  "(B)",  "(C)",  "(D)",  "(E)",
            "(F)",  "(G)",  "(H)",  "(I)",  "(J)",  "(K)",  "(L)",  "(M)",  "(N)",  "(O)",  "(P)",  "(Q)",  "(R)",
            "(S)",  "(T)",  "(U)",  "(V)",  "(W)",  "(X)",  "(Y)",  "(Z)",  "(a)",  "(b)",  "(c)",  "(d)",  "(e)",
            "(f)",  "(g)",  "(h)",  "(i)",  "(j)",  "(k)",  "(l)",  "(m)",  "(n)",  "(o)",  "(p)",  "(q)",  "(r)",
            "(s)",  "(t)",  "(u)",  "(v)",  "(w)",  "(x)",  "(y)",  "(z)",  "(0)",  "(11)", "(12)", "(13)", "(14)",
            "(15)", "(16)", "(17)", "(18)", "(19)", "(20)", "[1]",  "[2]",  "[3]",  "[4]",  "[5]",  "[6]",  "[7]",
            "[8]",  "[9]",  "[10]", "(0)"}[(c & 0xff) - 0x60];
    default: return "?";
    }
}
*/

}  // namespace cm
