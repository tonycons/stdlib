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
#include "Array.hh"
#include "commons/datastructs/ByteVector.hh"
#include <commons/core.hh>
#include <commons/core.hh>


namespace cm {
struct var;

/*
 -
 -
*/
// struct StringPOD {
//     union {
//         u32 data_lo;
//         u64 data;
//         char const* ptr;
//     } u;
//     union {
//         u64 data;
//         struct {
//             u64 length : 64 - 8;
//             u64 is_ptr : 8;
//         } attrib;
//     } m_;
// };

// .

struct [[gnu::packed]] EmbeddedString
{
    union {
        u8 bytes[sizeof(ByteVector) - 1];
        Vector<u8, sizeof(ByteVector) - 1> vector;
    };
};

static_assert(sizeof(EmbeddedString) == sizeof(ByteVector));


class String;

using StringCRef = String const&;


///
/// Inherit instead of making a type alias to have better compiler diagnostics.
///
struct StringValue : ArrayRef<char>
{
private:
    char _ch = '\0';

public:
    using ArrayRef<char>::ArrayRef;

    constexpr StringValue(char ch)
        : ArrayRef<char>(&this->_ch, 1), _ch(ch)
    {}
};

/*
 -
 -
*/
class String : public LinearIteratorComponent<String, char>,  //
               public IEquatable<String> {
    // ByteVector _bytes;

    // Union<ByteVector, EmbeddedString> _data;
    ByteVector _data;

public:
    constexpr static auto FORMAT_DELIMITER = '`';
    using Index = Union<usize, isize> const&;

    /**
     * Constructor from a string literal.
     * @param cstring The values
     */
    template<long long N>
    String(char const (&cstring)[N])
        : String(cstring, max(N - 1, 0))
    {}

    String();
    String(char c);
    String(char const* cstring, usize len);
    String(String const&) = default;
    String& operator=(String const&) = default;

    operator ArrayRef<char>() { return ArrayRef<char>(const_cast<char*>(cstr()), length()); }
    operator ArrayRef<char>() const { return ArrayRef<char>(const_cast<char*>(cstr()), length()); }
    operator StringValue() { return StringValue(const_cast<char*>(cstr()), length()); }
    operator StringValue() const { return StringValue(const_cast<char*>(cstr()), length()); }

    NODISCARD usize getActualIndex(Index i) const;

    NODISCARD char& operator[](Index i);

    NODISCARD char operator[](Index i) const;

    NODISCARD usize length() const;

    NODISCARD usize sizeBytes() const;

    NODISCARD bool empty() const { return length() == 0; }

    NODISCARD char const* data() const;

    NODISCARD char const* cstr() const { return this->data(); }

    NODISCARD bool equals(StringValue value) const;

    void outputString(String const& res, auto const& out) const
    {
        for (char c : res)
            out(c);
    }

    void ensureNullTermination();

    void erase(Index i, usize count) &;

    void insert(Index, StringValue) &;

    void replace(StringCRef substr, StringCRef replacement) &;


    void insertf(Index i, u64 value);


    /**
     * TODO
     */
    template<unsigned N, typename... Args>
    NODISCARD FORCEINLINE static String fmt(char const (&sFmt)[N], Args... args)
    {
        return _fmt(sFmt, {AnyRefT(args)...});
    }

    /**
     * TODO
     */
    NODISCARD FORCEINLINE static String valueOf(auto const& val)
    {
        String result;
        AnyRefT(val).outputString([&](char c) { result.append(c); });
        return result;
    }

    FORCEINLINE void append(StringValue s) & { return insert(length(), s); }
    FORCEINLINE void push(StringValue s) & { return insert(length(), s); }
    char pop() &;

    NODISCARD FORCEINLINE String replace(StringCRef s, StringCRef r) const { return String(*this).replace(s, r); }
    NODISCARD FORCEINLINE String replace(StringCRef s, StringCRef r) && { return (replace(s, r), *this); }

    NODISCARD FORCEINLINE String erase(Index i, usize n) const { return String(*this).erase(i, n); }
    NODISCARD FORCEINLINE String erase(Index i, usize n) && { return (erase(i, n), *this); }

    NODISCARD FORCEINLINE String insert(Index i, StringValue s) const { return String(*this).insert(i, s); }
    NODISCARD FORCEINLINE String insert(Index i, StringValue s) && { return (insert(i, s), *this); };

    NODISCARD FORCEINLINE String append(StringValue s) const { return String(*this).append(s); }
    NODISCARD FORCEINLINE String append(StringValue s) && { return (append(s), *this); }


    /**
     * Removes the final element if it is equal to some value.
     * @note If the element is not a simple data type and is removed, its destructor is called.
     * @param refValue The value to remove.
     */
    void removeSuffix(char refValue) &;

public:
    long long toInteger() const;
    double toDouble() const;

private:
    String(char const* ptr, int, int);
    static String _fmt(char const* sFmt, std::initializer_list<ConstRefWrapper<AnyRef>> const& objects);
};

template<typename Inheritor>
void Collection<Inheritor>::toString(String& res) const
{
    this->forEach([&](ElementType const& e) { res.append(String::valueOf(e)); });
    // map<String*>([](ElementType const& e) -> String* { return String::valueOf(e); }).join(",");
};

// static_assert(IsDerivedFrom<Array<char>, String>);

}  // namespace cm
