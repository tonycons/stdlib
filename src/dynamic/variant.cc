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

#include <commons/datastructs/variant.hh>
#include <unordered_map>

namespace cm {

std::unordered_map<u64, var (*)(var&, var&, var::Operator)> const& getBinaryOpTable();


constexpr u64 toClassIDPair(u32 id1, u32 id2) { return (u64)id1 | (((u64)id2) << 32); }

template<typename T0, typename T1>
FORCEINLINE u64 classIDCombination()
{
    return toClassIDPair(ClassOf<T0>.id, ClassOf<T1>.id);
}


var::var(std::initializer_list<var> initializer)
    : _data(new VariantData<Array<var>>(initializer))
{}

// var::~var() {
//     // Destructor called automatically by GC?
//     // if (auto destructor = getClass().destructor; destructor)
//     //     destructor(_pInstance);
// }

var var::binaryOp(var const& var1, Operator op) const
{

    //
    if (auto entry = getBinaryOpTable().find(toClassIDPair(this->getClass().id, var1.getClass().id));
        entry != getBinaryOpTable().end())
    {
        return (*entry).second((var&)*this, (var&)var1, op)._data->getInstance();
    }
    Assert(false, "Operation undefined");  // TODO
}

/*
 */
String var::toString() const
{


    auto rClass = getClass();
    auto _pInstance = _data->getInstance();

    if (rClass.isPrimitive && (rClass.primitive.isInteger || rClass.primitive.isFloatingPoint)) {
        switch (rClass.sizeBytes) {
        case 1:
            if (rClass.primitive.isInteger) {
                if (rClass.primitive.isSigned)
                    return String::fromSigned(*reinterpret_cast<__INT8_TYPE__*>(_pInstance), 10, false);
                else
                    return String::fromUnsigned(*reinterpret_cast<__UINT8_TYPE__*>(_pInstance), 10, false);
            }
            break;
        case 2:
            if (rClass.primitive.isInteger) {
                if (rClass.primitive.isSigned)
                    return String::fromSigned(*reinterpret_cast<__INT16_TYPE__*>(_pInstance), 10, false);
                else
                    return String::fromUnsigned(*reinterpret_cast<__UINT16_TYPE__*>(_pInstance), 10, false);
            }
            break;
        case 4:
            if (rClass.primitive.isInteger) {
                if (rClass.primitive.isSigned)
                    return String::fromSigned(*reinterpret_cast<__INT32_TYPE__*>(_pInstance), 10, false);
                else
                    return String::fromUnsigned(*reinterpret_cast<__UINT32_TYPE__*>(_pInstance), 10, false);
            } else if (rClass.primitive.isFloatingPoint) {
                return String::fromDouble(*reinterpret_cast<float*>(_pInstance));
            }
            break;
        case 8:
            if (rClass.primitive.isInteger) {
                if (rClass.primitive.isSigned)
                    return String::fromSigned(*reinterpret_cast<__INT64_TYPE__*>(_pInstance), 10, false);
                else
                    return String::fromUnsigned(*reinterpret_cast<__UINT64_TYPE__*>(_pInstance), 10, false);
            } else if (rClass.primitive.isFloatingPoint) {
                return String::fromDouble(*reinterpret_cast<float*>(_pInstance));
            }
            break;
        }
    }
    if (rClass == ClassOf<char const*>) {
        return String::copyFromPtrUnsafe(*reinterpret_cast<char const**>(_pInstance));
    }
    return String::fmt("\"`\"@`", getClass().name, _pInstance);
}


//


//


//


/**
 * Defines a function in the binary operations table for:
 * @tparam T0 - left hand type
 * @tparam T1 - right hand type
 * @param var0 - left hand variant
 * @param var1 - right hand variant
 */
template<typename T0, typename T1>
var TBinaryOp(var& var0, var& var1, var::Operator op)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wsign-compare"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#pragma clang diagnostic ignored "-Wswitch-enum"
#pragma clang diagnostic ignored "-Wfloat-equal"

    //
    auto t0 = *reinterpret_cast<T0*>(var0._data->getInstance());
    auto t1 = *reinterpret_cast<T1*>(var1._data->getInstance());

    switch (op) {

        //
    case var::Operator::ARRAY_INDEX: {
        if constexpr (IsPointer<T0> && IsInteger<T1>) {
            return t0[t1];
        }
        break;
    }
    case var::Operator::ADD: {
        if constexpr ((IsPointer<T0> || IsInteger<T0>) && (IsPointer<T1> || IsInteger<T1>)) {
            return t0 + t1;
        }
        break;
    }
    case var::Operator::SUBTRACT: {
        if constexpr ((IsPointer<T0> || IsInteger<T0>) && (IsPointer<T1> || IsInteger<T1>)) {
            return t0 - t1;
        }
        break;
    }
    case var::Operator::MULTIPLY: {
        if constexpr (IsMultipliable<T0, T1>) {
            return t0 * t1;
        }
        break;
    }
    case var::Operator::DIVIDE: {
        if constexpr (IsDivideable<T0, T1>) {
            return t0 / t1;
        }
        break;
    }
    case var::Operator::EQUAL: {
        if constexpr (IsEquatable<T0, T1>) {
            return t0 == t1;
        }
        break;
    }
    case var::Operator::NOT_EQUAL: {
        if constexpr (IsEquatable<T0, T1>) {
            return t0 != t1;
        }
        break;
    }
    case var::Operator::LESS_THAN: {
        if constexpr (IsComparableLT<T0, T1>) {
            return t0 < t1;
        }
        break;
    }
    case var::Operator::GREATER_THAN: {
        if constexpr (IsComparableGT<T0, T1>) {
            return t0 > t1;
        }
        break;
    }
    case var::Operator::GREATER_THAN_OR_EQUAL: {
        if constexpr (IsComparableGE<T0, T1>) {
            return t0 >= t1;
        } else if constexpr (IsEquatable<T0, T1> && IsComparableGT<T0, T1>) {
            return t0 == t1 || t0 > t1;
        } else if constexpr (IsComparableLT<T0, T1>) {
            return !(t0 < t1);
        } else if constexpr (IsEquatable<T0, T1> && IsComparableLE<T0, T1>) {
            return t0 == t1 || !(t0 <= t1);
        } else {
            break;
        }
    }
    case var::Operator::LESS_THAN_OR_EQUAL: {
        if constexpr (IsComparableLE<T0, T1>) {
            return t0 <= t1;
        } else if constexpr (IsEquatable<T0, T1> && IsComparableLT<T0, T1>) {
            return t0 == t1 || t0 < t1;
        } else if constexpr (IsComparableGT<T0, T1>) {
            return !(t0 > t1);
        } else if constexpr (IsEquatable<T0, T1> && IsComparableGE<T0, T1>) {
            return t0 == t1 || !(t0 >= t1);
        } else {
            break;
        }
    }
    default:
    }
    throw std::runtime_error("Operation undefined");
#pragma clang diagnostic pop
}


/**
 * Defines an entry in the binary operations table for:
 * @tparam T0 - left hand type
 * @tparam T1 - right hand type
 */
template<typename T0, typename T1>
auto makeBinaryOpTableEntry()
{
    return std::pair<u64, var (*)(var&, var&, var::Operator)>{classIDCombination<T0, T1>(), TBinaryOp<T0, T1>};
}

/**
 * Conveniently defines the binary operations for combinations of primitive types.
 */
#define MAPENTRY(T, T0, T1, T3, T4, T5, T6, T7, T8, T9, T00, T01, T02, T03, T04)                                       \
    makeBinaryOpTableEntry<T, T0>(), makeBinaryOpTableEntry<T, T1>(), makeBinaryOpTableEntry<T, T3>(),                 \
        makeBinaryOpTableEntry<T, T4>(), makeBinaryOpTableEntry<T, T5>(), makeBinaryOpTableEntry<T, T6>(),             \
        makeBinaryOpTableEntry<T, T7>(), makeBinaryOpTableEntry<T, T8>(), makeBinaryOpTableEntry<T, T9>(),             \
        makeBinaryOpTableEntry<T, T00>(), makeBinaryOpTableEntry<T, T01>(), makeBinaryOpTableEntry<T, T02>(),          \
        makeBinaryOpTableEntry<T, T03>(), makeBinaryOpTableEntry<T, T04>()


std::unordered_map<u64, var (*)(var&, var&, var::Operator)> const& getBinaryOpTable()
{
    static std::unordered_map<u64, var (*)(var&, var&, var::Operator)> lsMap{
        MAPENTRY(
            char, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long,
            long long, unsigned long long, float, double, long double),
        MAPENTRY(
            signed char, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long,
            unsigned long, long long, unsigned long long, float, double, long double),
        MAPENTRY(
            unsigned char, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long,
            unsigned long, long long, unsigned long long, float, double, long double),
        MAPENTRY(
            short, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long,
            long long, unsigned long long, float, double, long double),
        MAPENTRY(
            unsigned short, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long,
            unsigned long, long long, unsigned long long, float, double, long double),
        MAPENTRY(
            int, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long,
            long long, unsigned long long, float, double, long double),
        MAPENTRY(
            unsigned int, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long,
            unsigned long, long long, unsigned long long, float, double, long double),
        MAPENTRY(
            long, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long,
            long long, unsigned long long, float, double, long double),
        MAPENTRY(
            unsigned long, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long,
            unsigned long, long long, unsigned long long, float, double, long double),
        MAPENTRY(
            long long, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long,
            long long, unsigned long long, float, double, long double),
        MAPENTRY(
            unsigned long long, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long,
            unsigned long, long long, unsigned long long, float, double, long double),
        MAPENTRY(
            float, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long,
            long long, unsigned long long, float, double, long double),
        MAPENTRY(
            double, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long,
            long long, unsigned long long, float, double, long double),
        MAPENTRY(
            long double, signed char, char, unsigned char, short, unsigned short, int, unsigned int, long,
            unsigned long, long long, unsigned long long, float, double, long double),
    };
    return lsMap;
}


}  // namespace cm
