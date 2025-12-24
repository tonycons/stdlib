/*
   Copyright 2024 Anthony A. Constantinescu.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/

#pragma once
#ifndef __inline_core_header__
#warning Do not include this file directly; include "datastructs.hh" instead
#else


namespace cm {

template<typename T>
struct DLList;

/**
 * @brief Implements type erasure and list manipulation for DLList.
 */
namespace impl {
class DLList {
protected:
    template<typename T>
    friend struct ::cm::DLList;

    struct Container
    {
        struct Node
        {
            Node* next;
            Node* prev;
        };

        struct Iterator
        {
            Container* _list;
            Node* _curr;

            void _remove();
            void _insert(void const* mem);
            void* _get();
            void const* _get() const;
            bool _hasNext() const { return _curr != _list->_tail; }
            Iterator _next() const { return {_list, _curr->next}; }
            bool _hasPrev() const { return _curr != _list->_head; }
            Iterator _prev() const { return {_list, _curr->prev}; }
        };

        ~Container();
        friend struct Iterator;

        void clear();
        Iterator _begin();
        Iterator _begin() const;
        Iterator _end();
        Iterator _end() const;

        Node* _head;
        Node* _tail;
        ClassRef _objclass;
        u32 _length;
    };
};
}  // namespace impl

/**
 * @brief Typed doubly linked list
 * @tparam T type of the list elements
 */
template<typename T>
struct DLList : protected impl::DLList::Container
{
    struct Iterator : protected impl::DLList::Container::Iterator
    {
        auto& insert(T const& value = {}) { return (_insert(&value), *this); }
        auto& remove() { return (_remove(), *this); }
        auto prev() { return Iterator(_list, _curr->prev); }
        auto next() { return Iterator(_list, _curr->next); }
        auto hasNext() { return _hasNext(); }
        auto hasPrev() { return _hasPrev(); }
        Iterator& operator++() { return (*this = this->next()); }
        bool operator==(Iterator const& i) const { return _curr == i._curr; }
        T const& operator*() const noexcept { return *static_cast<T const*>(_get()); }
        T& operator*() noexcept { return *static_cast<T*>(_get()); }
        T const* operator->() const noexcept { return static_cast<T const*>(_get()); }
        T* operator->() noexcept { return static_cast<T*>(_get()); }

    private:
        friend struct DLList<T>;
        Iterator(impl::DLList::Container const* l, impl::DLList::Container::Node* n)
            : impl::DLList::Container::Iterator{const_cast<Container*>(l), n}
        {}
    };

    DLList()
        : impl::DLList::Container{{}, {}, ClassOf<T>, {}}
    {}

    using impl::DLList::Container::clear;

    constexpr auto length() const { return _length; }
    constexpr auto begin() { return Iterator(this, _head); }
    constexpr auto end() { return Iterator(this, nullptr); }
    constexpr auto begin() const { return Iterator(this, _head); }
    constexpr auto end() const { return Iterator(this, nullptr); }

    constexpr auto const& first() const { return *begin(); }
    constexpr auto const& last() const { return *end().prev(); }
    constexpr auto& first() { return *begin(); }
    constexpr auto& last() { return *end().prev(); }

    constexpr auto forEach(auto func)
    {
        for (auto it = begin(); it != end(); ++it) {
            func(*it);
        }
    }
};


}  // namespace cm

namespace cm::impl {

/*
 */
inline DLList::Container::~Container() { clear(); }

/*
 */
[[clang::noinline]]
inline void DLList::Container::clear()
{
    if (_objclass.destructor) {  // avoid evaluating the if statement inside the loop repeatedly
        for (auto n = _head; n != nullptr;) {
            auto tmp = n->next;
            UNSAFE(_objclass.destructor(reinterpret_cast<u8*>(n) + sizeof(Node)));
            delete[] n;
            n = tmp;
        }
    } else {
        for (auto n = _head; n != nullptr;) {
            auto tmp = n->next;
            delete[] n;
            n = tmp;
        }
    }
    _head = _tail = nullptr;
    _length = 0;
}

/*
 */
[[clang::noinline]]
inline void DLList::Container::Iterator::_insert(void const* mem)
{
    Assert(mem);
    Node* newNode;

    UNSAFE_BEGIN;
    {
        auto* newNodeBytes = new u8[sizeof(Node) + _list->_objclass.sizeBytes];  // TODO: alignment
        memcpy(newNodeBytes + sizeof(Node), mem, _list->_objclass.sizeBytes);
        newNode = reinterpret_cast<Node*>(newNodeBytes);
    }
    UNSAFE_END;

    if (_curr == nullptr) {  // iterator is at end
        newNode->prev = _list->_tail;
        newNode->next = nullptr;

        if (_list->_head == nullptr && _list->_tail == nullptr) {  // list is empty
            _list->_head = _list->_tail = newNode;
        } else {  // list is not empty
            _list->_tail->next = newNode;
            _list->_tail = newNode;
        }
    } else {
        newNode->prev = _curr->prev;
        newNode->next = _curr;

        if (_curr->prev) {
            _curr->prev->next = newNode;
        }
        _curr->prev = newNode;
        if (_curr == _list->_head) {
            _list->_head = _curr->prev;
        }
    }
    _list->_length++;
    _curr = newNode;
}

[[clang::noinline]]
inline void DLList::Container::Iterator::_remove()
{
    Assert(_curr);
    Node* tmp;

    if (_curr->prev) {
        _curr->prev->next = _curr->next;
    }
    if (_curr->next) {
        _curr->next->prev = _curr->prev;
    }
    if (_curr == _list->_head) {
        _list->_head = _curr->next;
    }
    if (_curr == _list->_tail) {
        _list->_tail = _curr->prev;
    }
    _list->_length--;
    tmp = _curr->next;
    delete[] _curr;
    _curr = tmp;
}

inline auto DLList::Container::Iterator::_get() const -> void const*
{
    Assert(_curr);
    return UNSAFE(reinterpret_cast<u8 const*>(_curr) + sizeof(Node));
}

inline auto DLList::Container::Iterator::_get() -> void*
{
    Assert(_curr);
    return UNSAFE(reinterpret_cast<u8*>(_curr) + sizeof(Node));
}

}  // namespace cm::impl
#endif
