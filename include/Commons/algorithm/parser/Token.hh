// #pragma once
// #include "Commons/datastructs/string.hh"

// namespace cm {

// /**
//  * @brief A lexical token: Consists of id (what this token represents) and value (what is stored in the token)
//  */
// class Token {
// public:
//     constexpr Token() = default;

// constexpr Token(String* value, int c)
//     : value(value), _c(c)
// {}
// constexpr bool operator==(Token const& t) const { return value == t.value && _c == t._c; }
// constexpr auto id() const { return _c; }
// constexpr bool is(int c) const { return _c == c; }
// String* value;

// private:
//     friend class Tokenizer;
//     int _c;
// };

// }  // namespace cm
