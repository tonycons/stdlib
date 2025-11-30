# Core Module

Provides the core functionality that other modules depend on.

There are several subsections within Core, which are all grouped under the <code>commons/core.hh</code> header. 
These individual headers should not be included by the user. This is a synopsis
of each subsection/feature set in Core.


* ### Base Definitions
    Defines some macros/special functions that are used practically everywhere and have no specific place, including:
    * Function attributes
    * The <code>UNSAFE(...)</code>,<code>UNSAFE_BEGIN</code>,<code>UNSAFE_END</code> macros for enclosing unsafe code.
    * An implementation of the <code>DEFER(...)</code> operator from the Zig programming language.
    * A trap/halt instruction
    * A pair structure
    * C++ new/delete operators

* ### Range
    Provides a Range iterable identical to Python's <code>range</code>. It allows iteration with a for-each loop over a set of integers, in either ascending or descending order, and with an optional step size. It is useful for avoiding off-by-one errors and minimizing the amount of boilerplate. For example,
    ```C++
    for (auto i : Range(0, 8, 2)) {
        ...
    }
    // As a shorter representation of:
    for (int i = 0; i < 8; i += 2) {
        ...
    }
    ```

    [See the Range documentation.](docs/core/range.md)

* ### Type Reflection
    Similar to C++'s *type_traits* and *concepts* headers, provides a set of 
    templates and concepts for static reflection, type checking, and type constraints.

    [See the Type Reflection documentation.](docs/core/reflection.md)

* ### Initializer List
    Pretty much a re-implementation of <code>std::initializer_list</code>, with some **constexpr** versions of *cstring* functions included.

* ### Tuple
    A custom, slightly cleaner implementation of a Tuple data type in C++.

* ### Function Reflection
    Similar to C++'s *type_traits* but for functions, allowing static reflection on functions to determine their return type, type of each argument, and more. This seems to be missing in the C++ standard library (at least, going a few years back) despite being entirely possible to implement.

    [See the Function Reflection documentation.](docs/core/reflection.md)

* ### Functions
    Provides several features related to higher-order functions and functional programming, similar to the C++ *functional* header.
    * <code>CFunction</code>- a cleaner alternative to C-style function pointers. It allows storing a reference to a C-style function or a non-capturing lambda. As shown here, 
        ```C++
        CFunction<int(float, float)> func;
        // Is equivalent to:
        int(*func)(float, float);
        ```
        While the C++ standard has <code>std::function</code>, the problem is that uses heap allocation, which creates unnecessary overhead when referencing C-style functions or non-capturing lambdas.<code>CFunction</code> is a better alternative. It is true that the C++ standard has also added something like <code>std::lightweight_function</code>, but only very recently.

    * <code>Function</code>- like <code>CFunction</code>, but allows referencing a capturing lambda. Can be considered identical to <code>std::function</code>, allocating data on the heap.

    [See the Functions documentation.](docs/core/functions.md)

* ### Integers
    Provides Rust-style signed and unsigned integer types, up to 256 bits, and including more uncommon types such as 24 and 48-bit integers.
    * <code>UintN\<Bits\></code>- An integer with a statically-variable number of bits
    * <code>UintRanged\<N\></code>- An integer with the minimum number of bits necessary to store **N** distinct states.
    Additionally, there is an implementation of various (**constexpr**) math functions for integers (which are sorely missing from the old C++ standard library)

    [See the Integer documentation.](docs/core/integer.md)

* ### Assert
    Provides panic and assertion functions, and the <code>SourceLocation</code> structure to record locations in source code. The *Assert* function automatically references the location where it fails, and can display a templated diagnostic message.

    [See the Assert documentation.](docs/core/assert.md)

* ### Union
    This is a [tagged union](https://en.wikipedia.org/wiki/Tagged_union) data structure, similar to that of the Rust Enum.
    It represents a piece of memory that can be occupied by one of several
    distinct types, with a tag that indicates which type is in use.

    [See the Union documentation.](docs/core/union.md) 

* ### Result
    This is a result monad structure- a value that hold either a "success" type or an "error" type, identical to <code>Result\<Ok,Err\></code> in Rust.

    [See the Result documentation.](docs/core/result.md)

* ### Optional
    This is an optional value- capable of either holding a value or nothing (<code>None</code>). Identical to the Rust <code>Option</code> or the C++ <code>std::optional</code> It is intended as a more robust, safe alternative to using **nullptr** to indicate the absence of a value (which hardly makes sense for non-pointer types anyway). Across languages, the use of **null** to indicate the absence of a value is referred to as a "billion-dollar mistake" by computer scientist Tony Hoare.

    [See the Option documentation.](docs/core/option.md)

* ### ArrayRef
    Provides a pointer+length wrapper called <code>ArrayRef</code>- A non-owning reference to an array. Additionally, implements an <code>Index</code> data structure which is incorporated in various data structures to allow for Python-style negative indexing (where the last element has index **-1**, second-to-last has index **-2**, and so on).

    [See the ArrayRef documentation.](docs/core/arrayref.md)