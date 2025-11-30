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
#include "array.hh"
#include "commons/datastructs/array_list.hh"
#include <commons/core.hh>


namespace cm {


class String;

struct Printable
{
    virtual ~Printable() = default;
    virtual void output(String& result) const = 0;
};

template<typename T>
struct PrintableT final : Printable
{
    T* ptr;

    PrintableT(T const& ref)
        : ptr(&const_cast<T&>(ref))
    {}
    void output(String& result) const override;
};

template<typename T>
[[maybe_unused]] PrintableT(T const&) -> PrintableT<T>;


///
/// A dynamic, heap-allocated string
/// @see docs/String.md
///
class String : public LinearIteratorComponent<String, char>,  //
               public IEquatable<String> {

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
    String(char const* str)
        : String(StringRef(str))
    {}
    String(char const* cstring, usize len);
    String(StringRef const& sv)
        : String(sv.data(), sv.length())
    {}

    NODISCARD String(String const&) = default;
    NODISCARD String& operator=(String const&) = default;

    NODISCARD operator ArrayRef<char>() { return ArrayRef<char>(const_cast<char*>(cstr()), length()); }
    NODISCARD operator ArrayRef<char>() const { return ArrayRef<char>(const_cast<char*>(cstr()), length()); }
    NODISCARD operator StringRef() { return StringRef(const_cast<char*>(cstr()), length()); }
    NODISCARD operator StringRef() const { return StringRef(const_cast<char*>(cstr()), length()); }

    NODISCARD usize getActualIndex(Index i) const;

    NODISCARD char& operator[](Index i);

    NODISCARD char operator[](Index i) const;

    NODISCARD usize length() const;

    NODISCARD usize sizeBytes() const;

    NODISCARD bool empty() const { return length() == 0; }

    NODISCARD char const* data() const;

    NODISCARD char const* cstr() const { return this->data(); }

    NODISCARD bool equals(StringRef value) const { return StringRef(*this).equals(value); }

    void ensureNullTermination();

    void erase(Index i, usize count) &;

    void insert(Index, StringRef) &;

    void replace(StringRef substr, StringRef replacement) &;


    void insertf(Index i, u64 value);


    /**
     * TODO
     */
    template<unsigned N, typename... Args>
    NODISCARD FORCEINLINE static String fmt(char const (&sFmt)[N], Args... args)
    {
        return _fmt(sFmt, {PrintableT(args)...});
    }

    /**
     * TODO
     */
    // NODISCARD FORCEINLINE static String valueOf(auto const& val) { return ToString(val); }

    FORCEINLINE void append(StringRef s) & { return insert(length(), s); }
    FORCEINLINE void push(StringRef s) & { return insert(length(), s); }
    char pop() &;

    NODISCARD FORCEINLINE String replace(StringRef s, StringRef r) const { return String(*this).replace(s, r); }
    NODISCARD FORCEINLINE String replace(StringRef s, StringRef r) && { return (replace(s, r), *this); }

    NODISCARD FORCEINLINE String erase(Index i, usize n) const { return String(*this).erase(i, n); }
    NODISCARD FORCEINLINE String erase(Index i, usize n) && { return (erase(i, n), *this); }

    NODISCARD FORCEINLINE String insert(Index i, StringRef s) const { return String(*this).insert(i, s); }
    NODISCARD FORCEINLINE String insert(Index i, StringRef s) && { return (insert(i, s), *this); };

    NODISCARD FORCEINLINE String append(StringRef s) const { return String(*this).append(s); }
    NODISCARD FORCEINLINE String append(StringRef s) && { return (append(s), *this); }


    ///
    /// Removes the final element if it is equal to some value.
    /// @note If the element is not a simple data type and is removed, its destructor is called.
    /// @param refValue The value to remove.
    ///
    void removeSuffix(char refValue) &;

public:
    long long toInteger() const;
    double toDouble() const;

private:
    struct Printable
    {
        virtual ~Printable() = default;
        virtual void output(String& result) const = 0;
    };

    template<typename T>
    struct PrintableT final : Printable
    {
        T* ptr;

        PrintableT(T const& ref)
            : ptr(&const_cast<T&>(ref))
        {}
        void output(String& result) const override
        {
            if constexpr (Constructible<String, T>) {
                result.append(*ptr);
            } else {
                OutputString(*ptr, [&](char c) { result.append(c); });
            }
        }
    };

    String(char const* ptr, int, int);
    static String _fmt(char const* sFmt, ArrayRef<ConstRefWrapper<Printable>> const& objects);
};


template<typename T>
void PrintableT<T>::output(String& result) const
{
    if constexpr (IsUnderlyingTypeOneOf<T, String, StringRef, char*, char>) {
        result.append(*ptr);
    } else {
        OutputString(*ptr, [&](char c) { result.append(c); });
    }
}


// static_assert(IsDerivedFrom<Array<char>, String>);

}  // namespace cm
