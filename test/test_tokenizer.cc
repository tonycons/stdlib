// #include <Commons/algorithm/Tokenizer.hh>
// #include <Commons/system/io/print.hh>
// #include <Commons/datastructs/Array.hh>
// using namespace cm;
// using namespace cm::parse;


// enum {
//     SPACE,
//     CH,
//     InnerExpr,
//     LEFT_PAR,
//     RIGHT_PAR,
//     DIV,
//     MUL,
//     DIGIT,
//     NUMBER,
//     ADD,
//     SUB,
//     IDMAX
// };

// // clang-format off
// static Tokenizer mainParser{Alphabet({
//     {SPACE, {' ', '\t', '\n', '\r'}},
//     {CH, {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '+', '-', '/', '*'}},
//     {RIGHT_PAR, {')'}},
//     {LEFT_PAR, {'('}},
// //
// })};
// static Tokenizer innerExprParser{Alphabet({
//     {DIGIT, {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'}},
//     {ADD, {'+'}},
//     {SUB, {'-'}},
//     {MUL, {'*'}},
//     {DIV, {'/'}}
// })};

// // clang-format on

// bool isArithmeticOperator(Token const& t)
// {
//     return t.is(ADD) || t.is(SUB) || t.is(MUL) || t.is(DIV);
// }

// auto middle(Token const&, Token const& middle, Token const&)
// {
//     return Array<Token, 1>{middle};
// }

// auto evalOperation(Token const& left, Token const& delim, Token const& right) -> Array<Token>
// {
//     io::print("\t\t\tEval operation ` ` `", left.value, delim.value, right.value);
//     if (left.id() == NUMBER && right.id() == NUMBER) {
//         double leftv = left.value.toDouble();
//         double rightv = right.value.toDouble();
//         double res;

// switch (delim.id()) {
// case DIV:
//     res = leftv / rightv;
//     break;
// case MUL:
//     res = leftv * rightv;
//     break;
// case ADD:
//     res = leftv + rightv;
//     break;
// case SUB:
//     res = leftv - rightv;
//     break;
// default:
//     return {Token("!", -1)};
// }
// io::println("= `", res);
// return {Token(String::valueOf(res), NUMBER)};
// } else {
// return {left, delim, right};
// }
// }

// bool evalInnerExpr(Token& innerExpr)
// {
//     innerExprParser.alphabetize(innerExpr.value).reduce(DIGIT, NUMBER);

// if (isArithmeticOperator(innerExprParser.getTokens().first()) ||
//     isArithmeticOperator(innerExprParser.getTokens().last()))
// {
//     return false;
// }
// auto r = innerExprParser
//              .repeat<replace_delimited_with<
//                  Left<NUMBER>, Precedence<Level<DIV, MUL>, Level<ADD, SUB>>, Right<NUMBER>, evalOperation>>()
//              .result();

// if (innerExpr.value != r) {
//     innerExpr.value = r;
//     return true;
// }
// return false;
// }

// void eval(String const& src)
// {
//     mainParser  //
//         .alphabetize(src)
//         //.print()
//         //.remove(SPACE)
//         //.print()
//         // .reduce(CH, InnerExpr)
//         // .print()
//         // .repeat<
//         //     for_each<InnerExpr, evalInnerExpr>,                          //
//         //     replace_enclosed<Left<LEFT_PAR>, Right<RIGHT_PAR>, middle>,  //
//         //     reduce<InnerExpr, InnerExpr>>()
//         // .print();
//         ;
// }


// extern "C" int test02()
// {
//     // clang-format off

// eval("(10+8)*6/(3+(4/3*(4+2)))");
// // clang-format on
// return 0;
// }
