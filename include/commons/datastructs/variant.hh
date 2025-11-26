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
#include <commons/datastructs/array.hh>
#include <commons/datastructs/string.hh>
#include "commons/gc/GCManaged.hh"
#include <commons/core.hh>


namespace cm {


struct VariantDataBase
{
protected:
    VariantDataBase(u32 uId)
        : id(uId)
    {}

    virtual ~VariantDataBase() = default;

public:
    u32 const id;
    virtual void* getInstance() const = 0;
};

template<typename T>
struct VariantData : public VariantDataBase, GCManaged<VariantData<T>>
{
    T _instance;


    VariantData(auto... args)
        : VariantDataBase(ClassOf<T>.id()), _instance(args...)
    {}

    ~VariantData() override = default;

    void* getInstance() const override { return const_cast<void*>(static_cast<void const*>(&_instance)); }
};

struct var
{

    /**
     * Constructs a variant as an array from an initializer list of homogenous types.
     * This variant is an array of type T.
     * @param x initializer
     */
    template<typename T>
    var(std::initializer_list<T> x)
        : _data(new VariantData<Array<T>>(x))
    {}

    /**
     * Constructs a variant as an array from an initializer list of non-homogenous types.
     * This variant is an array of variants.
     * @param x initializer
     */
    var(std::initializer_list<var> x);

    /**
     * Constructs a variant from a single value.
     * This variant is of type T.
     * @param x value
     */
    template<typename T>
    requires (IsSame<CVRefRemoved<T>, var>)
    _noinline var(T x)
        : _data(x._data)
    {}


    /**
     * Constructs a variant from a single value.
     * This variant is of type T.
     * @param x value
     */
    template<typename T>
    requires (!IsSame<CVRefRemoved<T>, var>)
    var(T x)
        : _data(new VariantData<T>(x))
    {}

    // _noinline
    // var(var const& x) : _pInstance(x._pInstance) {
    //     __builtin_memcpy_inline(this->sbo_, x.sbo_, sizeof(unsigned char) * 16);
    // }
    ~var() = default;


    template<typename T>
    inline bool is() const
    {
        if constexpr (IsSame<CVRefRemoved<T>, var>) {
            return true;
        } else {
            return this->getClass() == ClassOf<T>;
        }
    }

    template<typename T>
    inline T const& as() const
    {
        if (!this->is<T>()) {
            Assert(false, String::fmt("Variant: Invalid cast (\"`\" as \"`\")", getClass().name, ClassOf<T>.name));
        }
        return *reinterpret_cast<T const*>(this->_data->getInstance());
    }

    template<typename T>
    inline T& as()
    {
        if (!this->is<T>()) {
            Assert(false, String::fmt("Variant: Invalid cast (\"`\" as \"`\")", getClass().name, ClassOf<T>.name));
        }
        return *reinterpret_cast<T*>(this->_data->getInstance());
    }

    template<typename T>
    explicit operator T()
    {
        if (!this->is<T>()) {
            Assert(false, String::fmt("Variant: Invalid cast (\"`\" as \"`\")", getClass().name, ClassOf<T>.name));
        }
        return *reinterpret_cast<T*>(this->_data->getInstance());
    }


    enum class Operator {
        ARRAY_INDEX,
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        LESS_THAN,
        GREATER_THAN,
        EQUAL,
        NOT_EQUAL,
        GREATER_THAN_OR_EQUAL,
        LESS_THAN_OR_EQUAL,
        INCREMENT,
        INCREMENT_POSTFIX,
        DECREMENT,
        DECREMENT_POSTFIX
    };


    var unaryOp(Operator op);
    var binaryOp(var const& v, Operator op) const;

    // operator std::format_string<>() const;

    inline var operator[](var const& v) const { return binaryOp(v, Operator::ARRAY_INDEX); }

    inline var operator+(var const& v) { return binaryOp(v, Operator::ADD); }

    inline var operator-(var const& v) { return binaryOp(v, Operator::SUBTRACT); }

    inline var operator*(var const& v) { return binaryOp(v, Operator::MULTIPLY); }

    inline var operator/(var const& v) { return binaryOp(v, Operator::DIVIDE); }

    inline var operator==(var const& v) { return binaryOp(v, Operator::EQUAL); }

    inline var operator!=(var const& v) { return binaryOp(v, Operator::NOT_EQUAL); }

    inline var operator<(var const& v) { return binaryOp(v, Operator::LESS_THAN); }

    inline var operator>(var const& v) { return binaryOp(v, Operator::GREATER_THAN); }

    inline var operator>=(var const& v) { return binaryOp(v, Operator::GREATER_THAN_OR_EQUAL); }

    inline var operator<=(var const& v) { return binaryOp(v, Operator::LESS_THAN_OR_EQUAL); }

    inline var operator++() { return unaryOp(Operator::INCREMENT); }

    inline var operator--() { return unaryOp(Operator::DECREMENT); }

    inline var operator++(int);
    inline var operator--(int);

    String toString() const;

    inline Class const& getClass() const { return Class::fromID(_data->id); }

    VariantDataBase* _data;
};

static_assert(TriviallyCopyConstructible<var>);
static_assert(TriviallyDestructible<var>);

}  // namespace cm
