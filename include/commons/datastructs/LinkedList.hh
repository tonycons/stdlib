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
#include "commons/datastructs/Collection.hh"  // IWYU pragma: keep
#include <commons/core.hh>

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

// template<typename T>
// class UniChain;

// namespace CollectionType {
// template<typename T>
// class UniChain : public Collection<UniChain<T>> {
// public:
//     using ItsElementType = T;
//     using ItsType = ::cm::UniChain<T>;
// };
// }

}  // namespace cm
