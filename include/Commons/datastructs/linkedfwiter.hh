/*
   Copyright 2024 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

#pragma once
#include <Commons/core.hh>
#include <Commons/core.hh>
#include <Commons/core.hh>

namespace cm {

template<typename Derived, typename NodeType, typename ElementType>
struct LinkedFwIterable
{
    using Node = NodeType;

    struct Iterator : public IEquatable<Iterator>
    {
    public:
        Iterator(Derived* d, Node* node)
            : node(node), base(static_cast<LinkedFwIterable*>(d))
        {}

        constexpr inline operator ElementType&()
        {
            Assert(*this != base->end(), "End has no element");
            return this->node->value;
        }

        constexpr inline operator ElementType const&() const
        {
            Assert(*this != base->end(), "End has no element");
            return this->node->value;
        }

        constexpr inline Iterator operator*() const { return *this; }

        constexpr inline bool equals(ElementType c) const
        {
            Assert(*this != base->end(), "End has no element");
            return this->node->value == c;
        }

        constexpr inline bool equals(Iterator const& i) const { return this->node == i.node; }

        constexpr inline Iterator& operator++()
        {
            Assert(*this != base->end(), "Cannot advance iterator past end");
            this->node = this->node->next;
            return *this;
        }

        constexpr inline Iterator operator++(int)
        {
            Iterator result(*this, node);
            ++(*this);
            return result;
        }

        constexpr inline Iterator next()
        {
            Assert(*this != base->end(), "Cannot get next past end");
            return Iterator(this->node->next);
        }

    private:
        Node* node;
        LinkedFwIterable* base;
    };

    Iterator begin() { return static_cast<Derived&>(*this).begin(); }

    Iterator end() { return static_cast<Derived&>(*this).end(); }

    struct ForeachEnd
    {
        ForeachEnd(usize i)
            : i(i)
        {}

        usize then(auto callable)
        {
            if (i != 0)
                callable();
            return i;
        }

    private:
        usize i;
    };

    auto foreach (auto callable)
    {
        usize i = 0;
        for (ElementType& value : *this) {
            callable(value);
            i++;
        }
        return ForeachEnd(i);
    }
};

}  // namespace cm
