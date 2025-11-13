#pragma once
#ifdef __inline_core_header__

namespace cm {

namespace Errors {
struct Overflow
{};
struct BadFormat
{};
};  // namespace Errors

struct UnknownException
{};

struct NumberOverflowException
{};

struct StackOverflowException
{};

struct Forbid
{};

struct InvalidArgumentException
{};

struct BadFormatException
{};

struct OutOfMemoryException
{};
#define throws(...)

#define constexpr_throw(E)                                                                                             \
    do {                                                                                                               \
        if consteval {                                                                                                 \
            while (bool(#E))                                                                                           \
                ;                                                                                                      \
        } else {                                                                                                       \
            throw E;                                                                                                   \
        }                                                                                                              \
    } while (0)

namespace Exception {


// [[noreturn]]
// constexpr _noinline void raiseOverflow()
// {
//     throw NumberOverflowException{};
// }

}  // namespace Exception

}  // namespace cm
#endif
