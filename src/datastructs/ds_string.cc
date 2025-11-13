/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not
   use this file except in compliance with the License. You may obtain a copy of
   the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
   WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
   License for the specific language governing permissions and limitations under
   the License.
*/
#include <Commons/core.hh>  // IWYU pragma: keep
#include <Commons/system.hh>
#include "../resources/OneCharStringTable.hh"
#include <string.h>

extern "C" double pow(double b, double e);
extern "C" double log10(double);

namespace cm {


String::String()
    : _data("", 1)
{}

String::String(char c)
    : _data(Data::oneCharStringTable(c), 1)
{}

String::String(char const* cstring, usize len)
    : _data(cstring, len + 1)
{}

char const* String::data() const { return reinterpret_cast<char const*>(_data.data()); }
usize String::length() const { return _data.length() - 1; }
usize String::sizeBytes() const { return length() * sizeof(char); }

usize String::getActualIndex(Index i) const
{
    return i.match(
        [&](usize k) { return k; },
        [&](isize k) {  // Possible negative index
            return k < 0 ? usize(isize(length()) + k) : usize(k);
        });
}

char& String::operator[](Index i)
{
    UNSAFE(u8* r = &_data.data()[getActualIndex(i) * sizeof(char)]);
    return *reinterpret_cast<char*>(r);
}

//
char String::operator[](Index i) const
{
    UNSAFE(u8 const* r = &_data.data()[getActualIndex(i) * sizeof(char)]);
    return *reinterpret_cast<char const*>(r);
}

//
bool String::equals(StringValue value) const
{
    if (value.length() == this->length()) {
        return memcmp(value.data(), this->cstr(), this->length()) == 0;
    }
    return false;
}

//
char String::pop() &
{
    char c = (*this)[-1];
    erase(-1, 1);
    return c;
}

//
void String::erase(Index i, usize n) &
{
    usize actualIndex = getActualIndex(i);
    _data.erase(actualIndex * sizeof(char), n * sizeof(char));
}

//
void String::insert(Index i, StringValue s) &
{
    usize actualIndex = getActualIndex(i);
    _data.insert(actualIndex * sizeof(char), s.data(), s.sizeBytes());
}


long long String::toInteger() const
{
    auto result = 0, negate = 0;
    auto s = begin();

    while (*s == ' ' || *s == '\t') {
        ++s;
    }
    if (*s == '+' && *(s.next()) != '-') {
        ++s;
    }
    if (*s == '-') {
        negate = 1;
        ++s;
    }
    for (; *s != '\0'; ++s) {
        switch (*s) {
        case '0' ... '9':
            if (__builtin_mul_overflow(result, 10, &result) || __builtin_add_overflow(result, -('0' - *s), &result))
                return negate ? -2147483648 : 2147483647;
            continue;
        default: break;
        }
        break;
    }
    return negate ? -result : result;
}

//
double String::toDouble() const
{
    if (*this == "-inf") {
        return -__builtin_huge_val();
    } else if (*this == "inf") {
        return __builtin_huge_val();
    } else if (*this == "NaN") {
        return __builtin_nan("");
    }
    auto it = begin();
    while (it != last() && *it != '.') {
        ++it;
    }
    auto wholePart = double(toInteger());

    String a;
    while (it != last()) {
        ++it;
        a.append(*it);
    }
    if (auto fracInt = a.toInteger(); fracInt != 0) {
        wholePart += (double(fracInt) / (pow(10.0, __builtin_floor(log10(double(fracInt)))) * 10.0));
    }
    return wholePart;
}

//
UNSAFE_BEGIN
String String::_fmt(char const* sFmt, std::initializer_list<ConstRefWrapper<AnyRef>> const& objects)
{
    auto result = String();
    auto arg = objects.begin();
    auto fmtPtr = sFmt;

    while (*fmtPtr != '`' && *fmtPtr != '\0') {
        result.append(*fmtPtr);
        __builtin_printf("(1) Format result is: %s\n", result.cstr());
        fmtPtr++;
    }
    while (arg != objects.end()) {
        Assert(*fmtPtr == '`', "More arguments than specified in format string");
        (*arg)->outputString([&](char c) { result.append(c); });
        __builtin_printf("(2) Format result is: %s\n", result.cstr());
        arg++;
        fmtPtr++;
        for (; *fmtPtr != '`' && *fmtPtr != '\0'; fmtPtr++) {
            result.append(*fmtPtr);
            __builtin_printf("(3) Format result is: %s\n", result.cstr());
        }
    }
    __builtin_printf("(4) Format result is: %s\n", result.cstr());
    return result;
    UNSAFE_END
}

//
UNSAFE_BEGIN
void String::replace(String const& substr, String const& replacement) &
{
    auto* basePtr = cstr();
    auto* ptr = basePtr;
    while ((ptr = strstr(ptr, substr.cstr())) != nullptr) {
        __builtin_printf("Start from %s\n", ptr);
        [[assume(ptr - basePtr >= 0)]];
        auto index = size_t(ptr - basePtr);

        erase(index, substr.length());
        __builtin_printf("now its: %s\n", cstr());

        __builtin_printf("replacement length now: %zu\n", replacement.length());
        insert(index, replacement);  // problem is null terminator gets inserted.
        __builtin_printf("then: %s\n", cstr());

        if (basePtr != cstr()) {
            basePtr = cstr();
            ptr = basePtr + index;
        }
        ptr += max(replacement.length(), 1u);
    }
    UNSAFE_END
};

//
UNSAFE_BEGIN
void String::removeSuffix(char refValue) &
{
    if (length() != 0 && (*this)[length() - 1] == refValue)
        erase(length() - 1, 1);
    UNSAFE_END
}


}  // namespace cm
