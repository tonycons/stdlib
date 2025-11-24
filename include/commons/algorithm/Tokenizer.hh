// #pragma once
// #include "../core/initializer_list.hh"  // IWYU pragma: keep
// #include "commons/algorithm/parser/Alphabet.hh"
// #include "commons/algorithm/parser/Token.hh"
// #include "commons/datastructs/string.hh"
// #include "commons/datastructs/LinkedList.hh"


// namespace cm::parse {
// class Tokenizer;

// template<auto... ids>
// struct Level
// {
//     static std::initializer_list<int> const IDS;
// };

// template<typename... levels>
// struct Precedence
// {
//     static std::initializer_list<std::initializer_list<int>> const PRECEDENCE;
// };

// template<auto... ids>
// struct Left
// {
//     static std::initializer_list<int> const IDS;
// };

// template<auto... ids>
// struct Right
// {
//     static std::initializer_list<int> const IDS;
// };

// template<auto... ids>
// std::initializer_list<int> const Level<ids...>::IDS = {ids...};

// template<typename... levels>
// std::initializer_list<std::initializer_list<int>> const Precedence<levels...>::PRECEDENCE = {levels::IDS...};

// template<auto... ids>
// std::initializer_list<int> const Left<ids...>::IDS = {ids...};

// template<auto... ids>
// std::initializer_list<int> const Right<ids...>::IDS = {ids...};


// /**
//  * @brief .
//  */
// class Tokenizer {
// public:
//     Tokenizer(Alphabet const& alphabet)
//         : _alphabet(alphabet) {};
//     Tokenizer(Tokenizer const&) = delete;


// auto addAlphabet(int toID, std::initializer_list<char8_t> const& chars) -> Tokenizer&;
// auto alphabetize(String const& expr) -> Tokenizer&;

// auto remove(int id) -> Tokenizer&;
// auto print() -> Tokenizer&;
// String* result();

// /**
//  * Rule that evaluates a function for each token of identifier "id"
//  * @param id The identifier
//  * @param func The function
//  * @param optional parameter, sets a provided boolean to true if the rule was applied.
//  * @return Tokenizer&
//  */
// auto forEach(int id, auto func, bool* wasApplied = {}) -> Tokenizer&
// {
//     bool _;
//     if (!wasApplied) {
//         wasApplied = &_;
//     }
//     for (auto it = _tokens.begin(); it != _tokens.end(); ++it) {
//         if (it->is(id)) {
//             func(*it);
//         }
//     }
//     *wasApplied = false;
//     return *this;
// }

// /**
//  * Rule that evaluates a function for each token of identifier "id"
//  * @tparam id The identifier
//  * @tparam func The function
//  * @return True if the rule was applied.
//  */
// template<int id, auto func>
// bool _forEach()
// {
//     bool applied = false;
//     forEach(id, func, &applied);
//     return applied;
// }

// /**
//  * @brief Repeats applying series of rules until none of them apply.
//  * @tparam rules The rules
//  */
// template<auto... rules>
// auto& repeat()
// {
//     (void)_repeat<rules...>();
//     return *this;
// }

// /**
//  * @brief Repeats applying series of rules until none of them apply.
//  * @tparam rules The rules
//  */
// template<auto... rules>
// bool _repeat()
// {
//     unsigned long long max_iterations = 512;
//     bool anyreplaced;
//     do {
//         // std::cout << "iter " << max_iterations << "\n";
//         anyreplaced = false;
//         (
//             [&]() {
//                 anyreplaced |= (this->*(rules))();
//             }(),
//             ...);
//     } while (anyreplaced && max_iterations-- > 0);
//     return max_iterations != 512;
// }

// /**
//  * @brief Merge any sequence of one or more tokens of identifier "from" into exactly one token of identifier "to".
//  * For example, to merge a series of "digit" tokens into one "number" token.
//  * @param from from
//  * @param to to
//  * @param wasApplied optional parameter, sets a provided boolean to true if the rule was applied.
//  * @return Tokenizer&
//  */
// auto reduce(int from, int to, bool* wasApplied = nullptr) -> Tokenizer&
// {
//     String* nk = new String("");
//     auto const old_length = _tokens.length();
//     bool _;
//     if (!wasApplied) {
//         wasApplied = &_;
//     }
//     *wasApplied = false;
//     for (auto it = _tokens.begin(); it != _tokens.end(); ++it) {
//         if (it->is(from)) {
//             do {
//                 nk->append(it->value);
//                 it.remove();
//             } while (it != _tokens.end() && it->is(from));
//             *wasApplied = true;
//             it.insert(Token(nk, to));
//             nk->clear();
//         }
//     }
//     if (from == to && old_length == _tokens.length()) {
//         *wasApplied = false;
//     }
//     return *this;
// }

// /**
//  * @brief Merge any sequence of one or more tokens of identifier "from" into exactly one token of identifier "to".
//  * For example, to merge a series of "digit" tokens into one "number" token.
//  * @tparam From from
//  * @tparam To to
//  * @return True if the rule was applied.
//  */
// template<int From, int To>
// bool _reduce()
// {
//     bool applied;
//     reduce(From, To, &applied);
//     return applied;
// }


// /*
//  * Substitute a series of 3 tokens ( left + delim + right ) [right] with
//  * zero or more tokens. Where "delim" is any token of classifier "c_".
//  */
// template<typename Left, typename Right, auto func>
// auto replaceDelimited(auto const& delims) -> bool
// {
//     bool out_replaced = false;

// for (auto it = _tokens.begin(); it != _tokens.end();) {
//     if (linearSearch(delims, it->id()) == -1 || it == _tokens.begin()) {
//         ++it;
//         continue;
//     }
//     auto left = it.prev();
//     auto right = it.next();

// if (right == _tokens.end()) {
//     break;
// }
// auto const nt = func(*left, *it, *right);
// out_replaced = !(nt.length() == 3 && nt[0] == *left && nt[1] == *it && nt[2] == *right);

// left.remove();
// it.remove();
// right.remove();

// for (auto const& token : nt) {
//     right.insert(token);
//     ++it;
// }
// // if (it != _tokens.begin())
// //     --it;
// // print2(it);
// if (it == _tokens.end()) {
//     break;
// }
// //++it;
// }
// return out_replaced;
// }

// template<typename Left, typename P, typename Right, auto func>
// auto _replaceDelimitedWith() -> bool
// {
//     bool anyreplaced = false;
//     Array<int> dx;

// for (auto const& delims : P::PRECEDENCE) {
//     dx += delims;
//     anyreplaced |= replaceDelimited<Left, Right, func>(dx);
// }
// return anyreplaced;
// }

// template<typename Left, typename Right, auto func>
// bool _replaceEnclosed()
// {
//     // std::cout << "repenc\n";
//     bool out_replaced = false;
//     auto it = _tokens.begin();
//     unsigned long long max_iterations = 512;

// for (; it != _tokens.end() && (max_iterations--) != 0; ++it) {
//     if (it == _tokens.begin())
//         continue;

// auto left = it.prev();
// auto right = it.next();

// if (right == _tokens.end())
//     continue;

// if (linearSearch(Left::IDS, left->id()) == -1 || linearSearch(Right::IDS, right->id()) == -1)
//     continue;

// auto nt = func(*left, *it, *right);
// out_replaced = true;

// if (nt.length() == 3 && nt[0] == *left && nt[1] == *it && nt[2] == *right) {
//     out_replaced = false;
// }

// left.remove();
// it.remove();
// right.remove();

// for (auto const& token : nt) {
//     right.insert(token);
//     ++it;
// }
// // print();
// if (it == _tokens.end())
//     break;
// }
// return out_replaced;
// }

// bool _print()
// {
//     print();
//     return false;
// }

// auto const& getTokens() const { return _tokens; }

// private:
//     Alphabet _alphabet;
//     DLList<Token> _tokens;
// };

// template<auto... passes>
// constexpr inline auto repeat = &Tokenizer::_repeat<passes...>;

// template<int id, auto func>
// constexpr inline auto for_each = &Tokenizer::_forEach<id, func>;

// template<typename Left, typename Right, auto func>
// constexpr inline auto replace_enclosed = &Tokenizer::_replaceEnclosed<Left, Right, func>;

// template<typename Left, typename P, typename Right, auto func>
// constexpr inline auto replace_delimited_with = &Tokenizer::_replaceDelimitedWith<Left, P, Right, func>;

// template<int From, int To>
// constexpr inline auto reduce = &Tokenizer::_reduce<From, To>;

// template<int>
// constexpr inline auto print = &Tokenizer::_print;

// }  // namespace cm::parse


// #include "Tokenizer.inl"
