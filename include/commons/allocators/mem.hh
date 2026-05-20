/*
 *   Copyright 2026 Anthony A. Constantinescu.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *   in compliance with the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software distributed under the License
 *   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 *   or implied. See the License for the specific language governing permissions and limitations under
 *   the License.
 */


#pragma once

#include <commons/core.hh>

namespace cm {
struct Arena;
}

namespace cm {

struct Arena;

template<typename T>
constexpr T* arenaAlloc(Arena& arena, usize length);

template<typename T>
constexpr void arenaFree(Arena& arena, T* ptr, usize length);


template<typename T>
struct ControlBlockPolicy : public ArrayRef<T, T*>
{
    struct Block
    {
        Atomic<usize> ref_count{};
        usize length;
        T* data;
        Arena* arena;
    };

    Block* _block{};

    constexpr ControlBlockPolicy(usize initLength, Arena& arena)
        : _block(new (arenaAlloc<Block>(arena, 1)) Block{
              .ref_count = 1,
              .length = initLength,
              .data = cArrayDefaultConstruct(arenaAlloc<T>(arena, initLength), initLength),
              .arena = &arena,
          })
    {}

    constexpr ControlBlockPolicy(ControlBlockPolicy const& other)
        : _block(other._block)
    {
        _block->ref_count++;
    }

    constexpr ControlBlockPolicy& operator=(ControlBlockPolicy const& other)
    {
        ~ControlBlockPolicy();
        _block = other._block;
        _block->ref_count++;
        return *this;
    }

    constexpr ~ControlBlockPolicy()
    {
        Assert(_block->ref_count > 0);
        _block->ref_count--;

        if (_block->ref_count == 0) {
            Assert(_block->arena != nullptr);
            cArrayDestroy(_block->data, _block->length, false);
            arenaFree(*_block->arena, _block->data, _block->length);

            _block->data = nullptr;
            _block->length = 0;
            _block->arena = nullptr;

            arenaFree(*_block->arena, _block, 1);
        }
    }

    constexpr void reallocate(usize new_length)
    {
        Assert(new_length != 0);
        Assert(_block->data);
        auto* new_data = arenaAlloc<T>(*_block->arena, new_length);
        Assert(new_data);

        cArrayInit(new_data, _block->data, new_length, _block->length);
        cArrayDestroy(_block->data, _block->length, false);
        arenaFree(*_block->arena, _block->data, _block->length);

        _block->data = new_data;
        _block->_length = new_length;
    }

    constexpr T* data() { return _block->data; }
    constexpr T const* data() const { return _block->data; }
    constexpr usize length() const { return _block->length; }
};

template<typename T>
struct LinkedListPolicy
{
private:
    usize _length{};
    T* _data{};
    Arena* _arena{};
    LinkedListPolicy* _prev{};
    LinkedListPolicy* _next{};

public:
    constexpr LinkedListPolicy(usize initLength, Arena& arena)
        : _length(initLength),
          _data(cArrayDefaultConstruct(arenaAlloc<T>(arena, initLength), initLength)),
          _arena(&arena),
          _prev(nullptr),
          _next(nullptr)
    {}

    constexpr LinkedListPolicy(LinkedListPolicy const& other)
        : _length(other._length),
          _data(other._data),
          _arena(other._arena),
          _prev(const_cast<LinkedListPolicy*>(&other)),
          _next(other._next)
    {
        _prev->_next = this;
    }

    constexpr LinkedListPolicy& operator=(LinkedListPolicy const& other)
    {
        ~LinkedListPolicy();
        __builtin_memcpy_inline(this, &other, sizeof(*this));
        return *this;
    }

    constexpr ~LinkedListPolicy()
    {
        if (_prev) {
            _prev->_next = _next;
        }
        if (_next) {
            _next->_prev = _prev;
        }
        if (!_prev && !_next) {
            cArrayDestroy(_data, _length, false);
            arenaFree(*_arena, _data, _length);
        }
    }

    constexpr void reallocate(usize new_length)
    {
        T* new_data = cArrayDefaultConstruct(arenaAlloc<T>(*_arena, new_length), new_length);
        Assert(new_data);
        Assert(_data);
        cArrayInit(new_data, _data, new_length, _length);
        cArrayDestroy(_data, _length, false);
        arenaFree(*_arena, _data, _length);

        for (auto* prev_node = _prev; prev_node != nullptr; prev_node = prev_node->_prev) {
            $validate(prev_node);
            prev_node->_data = new_data;
            prev_node->_length = new_length;
        }
        _data = new_data;
        _length = new_length;

        for (auto* next_node = _next; next_node != nullptr; next_node = next_node->_next) {
            $validate(next_node);
            next_node->_data = new_data;
            next_node->_length = new_length;
        }
    }

    constexpr T* data() { return _data; }
    constexpr T const* data() const { return _data; }
    constexpr usize length() const { return _length; }
};


template<typename T>
struct Mem : public ArrayRef<T, LinkedListPolicy<T>>
{

    constexpr Mem(usize new_length, Arena& arena [[clang::lifetimebound]])
        : ArrayRef<T, LinkedListPolicy<T>>(new_length, arena)
    {}

    constexpr void reallocate(usize new_length)
    {
        if (this->length() != new_length) {
            this->_storage.reallocate(new_length);
        }
    }

    constexpr void expand(usize extra_size)
    {
        if (extra_size != 0) {
            reallocate(this->length() + extra_size);
        }
    }

    constexpr void shrink(usize reduced_by)
    {
        if (reduced_by != 0) {
            Assert(reduced_by >= this->length());
            reallocate(this->length() - reduced_by);
        }
    }
};

}  // namespace cm
