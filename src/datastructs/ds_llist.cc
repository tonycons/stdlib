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


#include "commons/datastructs/linked_list.hh"
#include <commons/core.hh>

extern "C" void* malloc(size_t);
extern "C" void* memcpy(void*, void const*, size_t);
extern "C" void free(void*);


namespace cm::impl {

/*
 */
DLList::Container::~Container() { clear(); }

/*
 */
void DLList::Container::clear()
{
    if (_objclass.destructor) {  // avoid evaluating the if statement inside the loop repeatedly
        for (auto n = _head; n != nullptr;) {
            auto tmp = n->next;
            UNSAFE(_objclass.destructor(reinterpret_cast<u8*>(n) + sizeof(Node)));
            free(n);
            n = tmp;
        }
    } else {
        for (auto n = _head; n != nullptr;) {
            auto tmp = n->next;
            free(n);
            n = tmp;
        }
    }
    _head = _tail = nullptr;
    _length = 0;
}

/*
 */
void DLList::Container::Iterator::_insert(void const* mem)
{
    Assert(mem);
    Node* newNode;

    UNSAFE({
        using u8ptr = u8*;
        auto* newNodeBytes = u8ptr(malloc(sizeof(Node) + _list->_objclass.sizeBytes));  // TODO: alignment
        memcpy(newNodeBytes + sizeof(Node), mem, _list->_objclass.sizeBytes);
        newNode = reinterpret_cast<Node*>(newNodeBytes);
    });

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

void DLList::Container::Iterator::_remove()
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
    free(_curr);
    _curr = tmp;
}

auto DLList::Container::Iterator::_get() const -> void const*
{
    Assert(_curr);
    return UNSAFE(reinterpret_cast<u8 const*>(_curr) + sizeof(Node));
}

auto DLList::Container::Iterator::_get() -> void*
{
    Assert(_curr);
    return UNSAFE(reinterpret_cast<u8*>(_curr) + sizeof(Node));
}

}  // namespace cm::impl
