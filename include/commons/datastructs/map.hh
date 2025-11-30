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
#include <commons/core.hh>
#include "sparse_array.hh"


namespace cm {


///
/// One implementation of a dictionary
///
/// 1) Based on a sparse array. The result of the hash function on a key is used as an index into the sparse array.
/// This naturally makes sense given that the hash function return integers that are "far apart"
/// Now since the sparse array takes a 64-bit index, a form of open addressing is used to resolve hash collisions:
/// The upper 32 bits of the index are the key hash
/// The lower 32 bits of the index are used to store each value that has the same key hash
/// This allows 2^32 objects to have the same key hash
///
template<typename K, typename V>
struct Map
{
    using HashFunction = CFunction<u32(K const&)>;
    constexpr static u32 defaultHashFunction(K const& k) { return Hash<Crc32>::hash(k); };


    Map(HashFunction const& hashFunc = defaultHashFunction)
        : _array(), _hashFunc(hashFunc)
    {}


    void put(K const& key, V const& value)
    {
        u32 keyHash = _hashFunc(key);
        u64 index = u64(keyHash) << 32;

        for (u32 j = 0; j < MAX_VALUE<u32>; j++) {
            u64 offset = index + j;
            Optional<Pair<K, V>> entry = _array.get(offset);

            if (!entry.hasValue()) {
                _array.set(offset, Pair<K, V>(key, value));
                break;
            }
        }
    }

    Optional<V&> get(K const& key)
    {
        u32 keyHash = _hashFunc(key);
        u64 index = u64(keyHash) << 32;

        for (u32 j = 0; j < MAX_VALUE<u32>; j++) {
            u64 offset = index + j;
            Optional<Pair<K, V>> entry = _array.get(offset);

            if (!entry.hasValue()) {
                return None;
            } else if (entry.hasValue() && key == entry.value().first) {
                return entry.value().second;
            }
        }
        return None;
    }

private:
    SparseArray<Pair<K, V>> _array;
    HashFunction _hashFunc;
};


}  // namespace cm
