# My "Commons" standard library

### A personal utility library for use in coding challenges, hacking, and future C++ projects. Implemented my own standard data structures, and several features from the Rust language.

### Motivation

C++ is my favorite language, and it faces a number of criticisms that I seek to challenge. This project considers an alternative design for a the standard library that would address some of those issues. 


* #### "C++ is unsafe and primitive"

   Only because codebases keep old habits and compilers allow code to be written like C.

   Clang offers the option to warn on using unsafe raw pointers and C-style arrays. Provided is an <code>UNSAFE(...)</code> block meant to embed code that is unsafe. Combined with the recommended option of treating warnings as errors with -Werror, this makes C++ identical to Rust in the requirement to explicitly mark unsafe code. Arrays are bounds checked. So, it is highly unlikely that a segfault could
   ever result outside of unsafe code.

   **Error handling:** Rather than throwing around ***nullptr*** or ***-1***, this project implements the <code>`Option<T>`</code> and <code>`Result<T,E>`</code> types from Rust.

   **Integer overflow:** (In progress) Integer types are represented using strongly typed enums, making unintentional implicit conversions impossible, and possess arithmetic operators that check for overflow.
   
   **RAII:**
   Implements the <code>DEFER</code> feature from Zig to allow arbitrary code executed on function exit. The result is less boilerplate from having to define custom classes and destructors every time you want to implement RAII for new things.

* #### "C++ is clunky"

   The majority of the gripes with C++ come not from the language itself, but from the design of the C++ standard library. It is a shame that the two are lumped together.
   
   The C++ standard library is cluttered with hundreds of global functions in the *std* namespace. Confusing names. Can you tell me what is the difference between <code>std::views</code> and <code>std::ranges::views</code>? I constantly have to
   consult the documentation, and even then, in which header file was it? There are too many header files. Which header files are included by other header
   files is *implementation-defined*. You may unknowingly write code that compiles
   once and then fails with a different compiler. 

   Bizarre operators like the << pipe operator and the new "|" operator introduced with the ranges module. Horrible compiler error messages resulting from the abuse of type aliases and excessive class template parameters.

   This project attempts to avoid some of these issues. For example, some type aliases of the form
   <code>
   using A = B;
   </code>
   can instead be replaced with
   <code>
   struct A : B {
      using B::B;
   };
   </code>
 
* #### "It takes forever to write code in C++, use Python to write code faster"

   In part, this is due to the difficulties in using the C++ standard library. But again-- this has nothing to do with the language itself. Except for a few things like list comprehensions, Python simply has more standard packages available, and others are easier to install. This project aims to cover a broader range of utilities just like the standard Python packages.

## Roadmap

| Completion         | Feature                         | Module |
| ------------------ | ------------------------------- | ------ |
| 🟩🟩🟩🟩🟩 100%   | Static reflection on types      |
| 🟩🟩🟩🟩🟩 100%   | Static reflection on functions  |
| 🟩🟩🟩🟩🟩 100%   | Debugging/Assertions            |
| 🟩🟩🟩🟩🟩 100%   | Tagged unions                   | 
| 🟩🟩🟩🟩🟩 100%   | Result/Option Type              | 
| 🟩🟩🟩🟩🟩 100%   | Tuple                           | 
| 🟩🟩🟩🟩⬛ 85%    | Integer utilities               |
| 🟨🟨🟨⬛⬛ 75%    | Floating-point utilities        |
| 🟨🟨🟨⬛⬛ 75%    | Random number generation        |
| 🟩🟩🟩🟩🟩 100%   | Polymorphic function wrapper    |
| 🟧🟧⬛⬛⬛ 40%    | Linear Algebra                  |
| ⬛⬛⬛⬛⬛ 0%     | Statistics                      |
| 🟧🟧⬛⬛⬛ 30%    | Search algorithms               |
| 🟩🟩🟩🟩⬛ 85%    | Sorting algorithms              |
| 🟨🟨🟨⬛⬛ 75%    | Strings                         |
| 🟩🟩🟩🟩🟩 100%   | Arrays                          |
| 🟩🟩🟩🟩⬛ 85%    | Linked Lists                    |
| 🟩🟩🟩🟩⬛ 85%    | Sparse Arrays                   |
| ⬛⬛⬛⬛⬛ 0%     | Dictionary                      |
|                    |                                 |
| 🟥⬛⬛⬛⬛ 20%    | Filesystem API                  |
| ⬛⬛⬛⬛⬛ 0%     | File format parsers             |
| ⬛⬛⬛⬛⬛ 0%     | Network API                     |
| 🟥⬛⬛⬛⬛ 1%     | Basic graphics API              |
| ⬛⬛⬛⬛⬛ 0%     | Basic audio API                 |





### What will not be implemented
#### Unicode-aware strings
There will be functions provided for conversion between different text encodings. However, if you want to process text with multi-byte characters, the only option will be to use a string of char32_t. I have several reasons for choosing this.
   * Indexing a character in a multi-byte encoding is an O(n) operation.
   * "Are two Unicode strings equal" is literally a Turing-undecidable problem -- because two Unicode strings that look visually identical could have different byte values and "invisible" markings/accents/etc. that set them apart. There are millions of edge cases, to which there exists no objective answer. Some of these problems include:
      * The unicode "á" (U+00E1) and "á" (the "a" + combining character '´') are the exact same thing, but have different byte values. Are they the same? Subjective answer.
      * There are over 10 different "dash" symbols. Then there are the ascii digits 0-9 and then some "digital led stylized" 0-9 digits. How do you define equality between characters that have different accent markings applied to them? You can add combining characters to Chinese, which shouldn´t even be possible. Should we ignore those when comparing Chinese? Subjective answer.
   * Having to possibly update the rules whenever new languages are added.
   * What even constitutes a "character" is entirely subjective. You can add a potentially infinite number of "combining characters" to a base character. Some people say these should be separate, others say they belong to the character.


## Documentation (TODO)

* [Option](docs/Option.md)
* [Union](docs/Union.md)
* [String](docs/String.md)
* [Array](docs/Array.md)
* [List](docs/List.md)
* [Collection](docs/Collection.md)
