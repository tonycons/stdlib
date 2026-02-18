### A C++ Standard library built from scratch, with its own implementations of standard data structures and several features from the Rust language.

This project explores an alternative design for a modern C++ standard library that is more suited towards embedded
systems, high performance, and memory safety. It is a work in progress, and serves more as a collection of ideas for the
time being.

* **Portability:** This is a header only library, which should work well with any build system. *While it is only
  guaranteed to work with the clang compiler, clang supports a large variety of architectures.*
* **Suitability for embedded systems:** No use of exceptions, heap allocation, or dynamic typing; able to run without a
  C++ runtime.
* **Only pay for what you use:** The only features that are compiled are the ones you use, so that smaller executables
  can be built.
* **Good Performance:** Performance critical code generation is checked using [Compiler Explorer](https://godbolt.org).
  Also, it's simple to check codegen for yourself on Compiler Explorer
  by [including the library over http](docs/httpinclude.md).
* **Memory Safety:** Various features inspired by Rust are implemented, such as [tagged unions](docs/core/union.md),
  Result, and [Optional](docs/Option.md) types, are provided to support memory safe programming. No features will invoke
  undefined behavior if used incorrectly.

****

## Setup/Requirements

The project requires C++26 to compile. The only requirements are **clang++ (>20.0)**, **cmake**, and [**ninja
**](https://ninja-build.org/).
Then, run the **setup** script and select the platform to build for.
Use the **run** script to build and run a sample.

## Feature Documentation (TODO)

* ### [Core Module](docs/core.md)
    * [Range](docs/core/range.md)
    * [Optional](docs/Option.md)
    * [Union](docs/core/union.md)
    * [Tuple](docs/Tuple.md)
    * [StringRef](docs/StringRef.md)
    * [ArrayRef](docs/ArrayRef.md)

* ### [Data Structure Module](docs/datastructs.md)
    * [Dynamic Array](docs/Array.md)
    * [Linked List](docs/List.md)
    * [Map](docs/Map.md)
    * [Queue](docs/Queue.md)
    * [String](docs/String.md)
    * [Stack](docs/Stack.md)

* ### [System Module](docs/system.md)
    * [Stream](docs/system/stream.md)
    * [Shell](docs/system/shell.md)

  ... And more to come.

## Roadmap

| Completion      | Feature                        | Module |
|-----------------|--------------------------------|--------|
| 🟩🟩🟩🟩🟩 100% | Static reflection on types     |
| 🟩🟩🟩🟩🟩 100% | Static reflection on functions |
| 🟩🟩🟩🟩🟩 100% | Debugging/Assertions           |
| 🟩🟩🟩🟩🟩 100% | Tagged unions                  |
| 🟩🟩🟩🟩🟩 100% | Result/Option Type             |
| 🟩🟩🟩🟩🟩 100% | Tuple                          |
| 🟩🟩🟩🟩⬛ 85%   | Integer utilities              |
| 🟨🟨🟨⬛⬛ 75%    | Floating-point utilities       |
| 🟨🟨🟨⬛⬛ 75%    | Random number generation       |
| 🟩🟩🟩🟩🟩 100% | Polymorphic function wrapper   |
| 🟧🟧⬛⬛⬛ 40%     | Linear Algebra                 |
| ⬛⬛⬛⬛⬛ 0%        | Statistics                     |
| 🟧🟧⬛⬛⬛ 30%     | Search algorithms              |
| 🟩🟩🟩🟩⬛ 85%   | Sorting algorithms             |
| 🟨🟨🟨⬛⬛ 75%    | Strings                        |
| 🟩🟩🟩🟩🟩 100% | Arrays                         |
| 🟩🟩🟩🟩⬛ 85%   | Linked Lists                   |
| 🟩🟩🟩🟩⬛ 85%   | Sparse Arrays                  |
| ⬛⬛⬛⬛⬛ 0%        | Dictionary                     |
|                 |                                |
| 🟥⬛⬛⬛⬛ 20%      | Filesystem API                 |
| ⬛⬛⬛⬛⬛ 0%        | File format parsers            |
| ⬛⬛⬛⬛⬛ 0%        | Network API                    |
| 🟥⬛⬛⬛⬛ 1%       | Basic graphics API             |
| ⬛⬛⬛⬛⬛ 0%        | Basic audio API                |

https://news.ycombinator.com/item?id=40555787

Notes for using CLion.

1. Disable "Run with PTY" (Best for Linux/WSL)
   The issue often stems from CLion forcing Pseudo-Terminal (PTY) emulation, which causes issues with how characters are
   buffered and displayed.
   Press Ctrl+Shift+A (or Help -> Find Action) and search for Registry...
   In the registry, search for run.processes.with.pty.
   Uncheck the box next to run.processes.with.pty



