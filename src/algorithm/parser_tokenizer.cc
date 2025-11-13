// #include <Commons/algorithm/Tokenizer.hh>
// #include <Commons/system/io/print.hh>


// namespace cm::parse {


// Tokenizer& Tokenizer::alphabetize(String const& expr)
// {
//     _tokens.clear();

// // io::println(expr);
// for (auto it = expr.begin(); it != expr.end(); ++it) {
//     _tokens.end().insert(Token(String::valueOf(*it), _alphabet.idOf(*it)));
// }
// return *this;
// }

// Tokenizer& Tokenizer::remove(int id)
// {
//     (void)id;
//     for (auto it = _tokens.begin(); it != _tokens.end(); ++it) {
//         // io::println(it->value);
//         //  if (it->is(id)) {
//         //      do {
//         //          it.remove();
//         //      } while (it != _tokens.end() && it->is(id));
//         //  }
//     }
//     return *this;
// }

// Tokenizer& Tokenizer::print()
// {
//     unsigned i = 0;
//     for (auto const& t : _tokens) {
//         io::print("``\033[0m", (i ? "\033[43m" : "\033[42m"), t.value);
//         i ^= 1;
//     }
//     io::println("");
//     return *this;
// }

// String* Tokenizer::result()
// {
//     String* r = new String();
//     for (auto const& t : _tokens) {
//         r->append(t.value);
//     }
//     return r;
// }

// }  // namespace cm::parse
