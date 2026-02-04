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

// clang-format off
namespace impl {
template<typename T>
struct FixGotchaType_ { using Type = T; };
template<unsigned N, typename T>
struct FixGotchaType_<T const (&)[N]> { using Type = ArrayRef<T>; };
template<unsigned N>
struct FixGotchaType_<char const (&)[N]>{ using Type = StringRef; };
template<typename T> 
using FixGotchaType = typename FixGotchaType_<T>::Type;
}  // namespace impl
// clang-format on


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
    Optional<Tuple<K, V>> _table[N];


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
    constexpr explicit FixedMap(Args&&... keysValues)
    {
        auto f = [&]<int I>(this auto&& self) constexpr {
            if constexpr (I < sizeof...(Args)) {
                add(Tuple<K, V>(keysValues...[I], keysValues...[I + 1]));
                self.template operator()<I + 2>();
            }
        };
        f.template operator()<0>();
    }

    ///
    /// Initialize a map from a list of tuples
    ///
    constexpr explicit FixedMap(ArrayRef<Tuple<K, V>> const& tuples)
    {
        for (auto const& tuple : tuples) {
            add(tuple);
        }
    }

    ///
    /// Access a value associated with the given key. If there is no such key-value pair, returns None
    ///
    constexpr Optional<V> operator[](K const& key) const
    {
        auto i = _hashFunc(key) % N;
        if (!_table[i].hasValue()) {
            return None;
        }
        if (auto const& ref = _table[i].ref(); ref.first == key) {
            return ref.second;
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
            if (_table[j].hasValue() && _table[j].ref().first == key) {
                return _table[j].val().second;
            }
            ++c;
        } while (_table[j].hasValue());
        return None;
    }

    ///
    /// Insert a new key-value mapping. If the key-value mapping already exists, the previous is replaced with the new.
    ///
    constexpr void add(Tuple<K, V> const& tuple)
    {
        auto const& key = tuple.first;
        auto i = _hashFunc(key) % N;
        if (!_table[i].hasValue()) {
            _table[i] = tuple;
            return;
        }
        // find another spot using open addressing
        auto j = i;
        auto c = 0u;
        do {
            if (_table[j].hasValue() && key == _table[j].ref().first) {  // Replace duplicate
                _table[j] = tuple;
                return;
            }
            ++j;
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
// template<typename K, typename V>
// FixedMap(ArrayRef<Tuple<K, V>> const& values) -> FixedMap<K, V, values.length()>;

template<typename K, typename V>
FixedMap(K&&, V&&) -> FixedMap<impl::FixGotchaType<K>, impl::FixGotchaType<V>, 1>;

template<typename K, typename V, typename... KVN>
FixedMap(K&&, V&&, KVN&&...) -> FixedMap<impl::FixGotchaType<K>, impl::FixGotchaType<V>, (sizeof...(KVN) / 2) + 1>;

}  // namespace cm
#endif
