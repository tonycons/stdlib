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
#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else

namespace cm {

template<typename T>
struct Generator
{};


template<typename Collection, typename CollectionFilterIterator, typename Predicate>
struct FilterGenerator
{
    Collection const* _collection;
    Predicate const* _predicate;

    struct FilterIterator
    {
        FilterGenerator const* _filter;
        CollectionFilterIterator _curr;

        constexpr inline FilterIterator(FilterGenerator const* filter, bool is_begin)
            : _filter(filter), _curr(is_begin ? _filter->_collection->begin() : _filter->_collection->end())
        {
            // Set curr to the first element matching predicate
            while (_curr != _filter->_collection->end() && !(*_filter->_predicate)(*_curr)) {
                UNSAFE_BEGIN;
                ++_curr;
                UNSAFE_END;
            }
        }

        constexpr ~FilterIterator() = default;
        constexpr FilterIterator(FilterIterator const&) = default;


        constexpr inline bool operator==(FilterIterator const& other) const
        {
            if (this == &other) {
                return true;
            }
            return this->_curr == other._curr;
        }

        constexpr inline FilterIterator& operator++()
        {
            // Go to the next element matching predicate
            while (_curr != _filter->_collection->end() && !(*_filter->_predicate)(*_curr)) {
                UNSAFE_BEGIN;
                ++_curr;
                UNSAFE_END;
            }
            return *this;
        }

        constexpr inline auto& operator*() const { return *_curr; }
    };

    constexpr inline FilterIterator begin() const { return FilterIterator(this, true); }
    constexpr inline FilterIterator end() const { return FilterIterator(this, false); }
    constexpr inline FilterGenerator(Collection const* collection, Predicate const* predicate)
        : _collection(collection), _predicate(predicate)
    {}
};


constexpr auto Filter(auto const& iterable, auto const& predicate)
{
    return FilterGenerator<
        CVRefRemoved<decltype(iterable)>, decltype(iterable.begin()), CVRefRemoved<decltype(predicate)>>(
        &iterable, &predicate);
}


}  // namespace cm
#endif
