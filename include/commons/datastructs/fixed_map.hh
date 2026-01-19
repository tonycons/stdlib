/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: core/union
   Description:
    Implements a Union type that can hold one of several types at a time.
    The Union type uses a strong, medium, and weak matching rule to determine which type to initialize the union with,
    based on the type of the value passed to the constructor.

    The functionality is similar to that of Rust Enum.
    It follows the exact same memory layout to the Rust Enums-
    Essentially "struct { u8 tag; union { .... } u; };"
    Except while Rust uses a 16-bit integer as a tag, this can use a byte as the tag if there are less than 255 types
    See https://patshaughnessy.net/2018/3/15/how-rust-implements-tagged-unions
*/

#pragma once
#ifdef __inline_core_header__

namespace cm {


namespace impl {
template<typename T>
struct FixGotchaType
{
    using Type = T;
};

template<unsigned N, typename T>
struct FixGotchaType<T const (&)[N]>
{
    using Type = ArrayRef<T>;
};

template<unsigned N>
struct FixGotchaType<char const (&)[N]>
{
    using Type = StringRef;
};


}  // namespace impl

// The purpose of this is to fix a problem I cannot describe in one sentence.
// If you initialize a map like this;   FixedMap("hello", 1), template deduction would normally create a FixedMap with
// key type as const char*. This will cause problems with places in the hash map where keys are compared because
// comparing two "const char*" for equality will compare the pointers, not the string. So to have it behave properly,
// the key would need to be of type StringRef. When template deduction results in one of these types that needs to
// change, such as const char*, FixGotchaType aliases it to the type it should be for proper behavior, such as
// StringRef.
template<typename T>
using FixGotchaType = typename ::cm::impl::FixGotchaType<T>::Type;

/**
 * A fixed-capacity container of key value pairs.
 * Keys are mapped to values with a provided hash function, and uses open addressing to resolve hash collisions.
 * @tparam K The type of key
 * @tparam V The type of value
 * @tparam N The capacity (maximum number of key value pairs that can be stored)
 */
template<typename K, typename V, unsigned N>
struct FixedMap
{
private:
    Array<Optional<Tuple<K, V>>, N> _table;


public:
    using HashFunction = CFunction<u32(K const&)>;
    constexpr static u32 defaultHashFunction(K const& k) { return Hash<Crc32>::hash(k); };
    constexpr static HashFunction _hashFunc = defaultHashFunction;

    ///
    /// Initialize a FixedMap from an argument list of key-value pairs.
    /// The order of the arguments goes like this: (key, value, key, value, key, value, ...)
    /// If the template parameter N is not specified in the type signature, the capacity of the map will be equal to
    /// however many key-value pairs were listed.
    ///
    template<typename... Args>
    requires ((sizeof...(Args) % 2) == 0)
    FixedMap(Args&&... keysValues)
    {
        auto f = [&]<int I>(this auto&& self) {
            if constexpr (I < sizeof...(Args)) {
                add(Tuple<K, V>(keysValues...[I], keysValues...[I + 1]));
                self.template operator()<I + 2>();
            }
        };
        f.template operator()<0>();
    }

    FixedMap(ArrayRef<Tuple<K, V>> const& tuples)
    {
        for (auto const& tuple : tuples) {
            add(tuple);
        }
    }

    ///
    /// Access a value associated with the given key. If there is no such key-value pair, returns None
    ///
    constexpr Optional<V> operator[](K const& key)
    {
        auto i = _hashFunc(key) % N;
        if (!_table[i].hasValue()) {
            return None;
        } else if (auto const& ref = _table[i].ref(); ref.template get<0>() == key) {
            return ref.template get<1>();
        }
        auto j = i;
        auto c = 0u;
        do {
            j++;
            if (j >= N) {
                j = 0;
            }
            if (c == N) {
                break;
            }
            if (_table[j].hasValue() && _table[j].ref().template get<0>() == key) {
                return _table[j].val().template get<1>();
            }
            ++c;
        } while (_table[j].hasValue());
        return None;
    }

    constexpr void add(Tuple<K, V> const& tuple)
    {
        auto const& key = tuple.template get<0>();
        auto i = _hashFunc(key) % N;
        if (!_table[i].hasValue()) {
            _table[i] = tuple;
            return;
        }
        // find another spot using open addressing
        auto j = i;
        auto c = 0u;
        do {
            if (_table[j].hasValue() && key == _table[j].ref().template get<0>()) {  // Replace duplicate
                _table[j] = tuple;
                return;
            }
            j++;
            if (j >= N) {
                j = 0;
            }
            if (c == N) {
                break;
            }
            if (!_table[j].hasValue()) {
                _table[j] = tuple;
                return;
            }
            ++c;
        } while (_table[j].hasValue());
        // table is full
        Assert(false);
    }

    constexpr auto capacity() { return N; }

    constexpr void add(K const& key, V const& value) { add(Tuple<K, V>(key, value)); }
};


// template<typename T>
// FixedMap(T&& tuple) -> FixedMap<TupleElement<T, 0>, TupleElement<T, 1>, 1>;

// template<IsTuple T1, IsTuple... TN>
// FixedMap(T1&& arg1, TN&&... argN) -> FixedMap<TupleElement<T1, 0>, TupleElement<T1, 1>, sizeof...(TN) + 1>;

template<typename K, typename V>
FixedMap(K&&, V&&) -> FixedMap<FixGotchaType<K>, FixGotchaType<V>, 1>;

template<typename K, typename V, typename... KVN>
FixedMap(K&&, V&&, KVN&&...) -> FixedMap<FixGotchaType<K>, FixGotchaType<V>, (sizeof...(KVN) / 2) + 1>;

// template<typename K, typename V>
// FixedMap(ArrayRef<Tuple<K, V>> const& values) -> FixedMap<K, V, values.length()>;

}  // namespace cm
#endif
