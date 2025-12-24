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


namespace cm {

namespace CollectionMethods {

// template<typename Inheritor, bool InheritorIsClass = IsClass<typename Inheritor::ItsElementType>>
// struct ForEach;

// template<typename Inheritor>
// struct ForEach<Inheritor, false> {
//     template<typename T>
//     using Derived = Inheritor::template ItsType<T>;
//     using ElementType = typename Inheritor::ItsElementType;

// /**
//  * @brief Executes func(x) for each x in the collection.
//  * @param func A function to execute for each x in the collection that does not modify x.
//  * @return The collection (to allow chaining function calls)
//  */
// auto& forEach(auto func) const {
//     for (auto const& element : static_cast<Derived<ElementType> const&>(*this))
//         func(element);

// return const_cast<Derived<ElementType>&>(static_cast<Derived<ElementType> const&>(*this));
// }
// };

// // Define an additional overload of the forEach function allowing class member functions to be used on the elements
// template<typename Inheritor>
// struct ForEach<Inheritor, true> : ForEach<Inheritor, false> {
//     template<typename T>
//     using Derived = Inheritor::template ItsType<T>;
//     using ElementType = typename Inheritor::ItsElementType;

// /**
//  * @brief Executes x.func() for each x in the collection.
//  * @param func A member function to execute for each x in the collection that does not modify x.
//  * @return The collection (to allow chaining function calls)
//  */
// auto& forEach(auto (ElementType::*method)()) const {
//     for (auto const& element : static_cast<Derived<ElementType> const&>(*this))
//         (element.*(method))();

// return const_cast<Derived<ElementType>&>(*this);
// }
// };


template<typename Inheritor, bool InheritorIsClass = IsClass<typename Inheritor::ItsElementType>>
struct Map;

template<typename Inheritor>
struct Map<Inheritor, false>
{
    template<typename T>
    using Derived = Inheritor::template ItsType<T>;
    using ElementType = typename Inheritor::ItsElementType;

    /**
     * Creates a new collection populated with the results of calling a provided function on every
     * element in the collection.
     * @param fnMapper A function to execute for each element in the array. Its return value is added
     * as a single element in the new array.
     * @return A new collection containing the results
     */
    auto& map(auto fnMapper, Derived<decltype(fnMapper(declval<ElementType>()))> const& out = {}) const
    {

        static_cast<Derived<ElementType> const&>(*this).forEach(
            [&](ElementType const& e) { const_cast<ConstRemoved<decltype(out)>&>(out) += fnMapper(e); });

        return const_cast<ConstRemoved<decltype(out)>&>(out);
    }
};

// Define an additional overload of the map function allowing class member functions to be used as mappers
template<typename Inheritor>
struct Map<Inheritor, true> : Map<Inheritor, false>
{
    template<typename T>
    using Derived = Inheritor::template ItsType<T>;
    using ElementType = typename Inheritor::ItsElementType;

    /**
     * Creates a new collection populated with the results of calling a provided function on every
     * element in the collection.
     * @param method A const member function to apply on each element.
     * For example, items->map(&Item::price) would result a collection containing item.price() for each item in items.
     * @return A new collection containing the results
     */
    auto&
    map(auto (ElementType::*method)() const,
        Derived<decltype((declval<ElementType>().*(method))())> const& out = {}) const
    {

        static_cast<Derived<ElementType> const&>(*this).forEach(
            [&](ElementType const& e) { const_cast<ConstRemoved<decltype(out)>&>(out) += (e.*(method))(); });

        return const_cast<ConstRemoved<decltype(out)>&>(out);
    }
};

template<typename Inheritor>
struct Join
{
    template<typename T>
    using Derived = Inheritor::template ItsType<T>;
    using ElementType = typename Inheritor::ItsElementType;

    /**
     * @brief Performs a join operation with a delimiter, "summing" all the elements into a single element as if there
     * also existed an element of value 'delim' between every element in the collection.
     * @param delim The delimiter
     * @return The sum
     */
    auto join(auto const& delim) const
    {
        ElementType result;
        auto derived = static_cast<Derived<ElementType> const&>(*this);
        auto i = 0ull;

        derived.forEach([&](ElementType const& e) {
            result += e;
            if (i < derived.length() - 1) {
                result += delim;
            }
            i++;
        });
        // if (static_cast<Derived<ElementType> const&>(*this).length() != 0) {
        //     auto last = static_cast<Derived<ElementType> const&>(*this).end();
        //     auto it = static_cast<Derived<ElementType> const&>(*this).begin();
        //     --last;

        // while (it != last) {
        //     const_cast<ElementType&>(result) += *it;
        //     const_cast<ElementType&>(result) += delim;
        //     ++it;
        // }
        // const_cast<ElementType&>(result) += *it;
        // }
        return result;
    }
};

template<typename Inheritor>
class Invert {
    using ElementType = typename Inheritor::ItsElementType;
    using Derived = typename Inheritor::template ItsType<ElementType>;

public:
    auto& invert()
    {

        auto len = static_cast<Derived&>(*this).length();

        if (len != 0) {
            auto l = static_cast<Derived&>(*this).begin();
            auto r = static_cast<Derived&>(*this).end();
            --r;

            for (auto i = decltype(len){}; i < (len / 2); i++) {
                auto tmp = *l;
                *l = *r;
                *r = tmp;
                --r;
                ++l;
            }
        }
        return static_cast<Derived&>(*this);
    }
};

}  // namespace CollectionMethods


template<typename Inheritor>
struct Collection : CollectionMethods::Map<Inheritor>,
                    CollectionMethods::Join<Inheritor>,
                    CollectionMethods::Invert<Inheritor>
{


    using ElementType = typename Inheritor::ItsElementType;
    using Derived = typename Inheritor::template ItsType<ElementType>;
    using RefElementType = ElementType&;


    void toString(String& res) const;

    // using RefElementType = ElementType&;

    // template<typename T = ElementType>
    // using Derived = Inheritor::template ItsType<T>;

    // template<typename T = ElementType>
    // using RefDerived = Derived<T>&;

    // template<typename T = ElementType>
    // using RefConstDerived = Derived<T> const&;

    // template<typename T = ElementType>
    // using PtrDerived = Derived<T>*;

    // template<typename T = ElementType>
    // using PtrConstDerived = Derived<T> const*;

    /*

    [[no_unique_address]] struct : ComputedProperty<Collection<Inheritor>> {
        auto const* operator->() const {
            return PtrConstDerived<ElementType>(
                ComputedProperty<Collection<Inheritor>>::container(&Collection<Inheritor>::derived));
        }
        auto* operator->() {
            return PtrDerived<ElementType>(
                ComputedProperty<Collection<Inheritor>>::container(&Collection<Inheritor>::derived));
        }
        RefConstDerived<ElementType> operator*() const { return *this->operator->(); }
        RefDerived<ElementType> operator*() { return *this->operator->(); }
    } derived;
    */

    // auto& join(auto const& delim, ElementType const& = {});

    /**
     * @brief Reverses the order of elements in this collection.
     * @return A reference to this collection (used to chain function calls)
     */
    // auto& invert();
};
}  // namespace cm
