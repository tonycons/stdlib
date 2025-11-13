# Union


This is a [tagged union](https://en.wikipedia.org/wiki/Tagged_union) data structure, similar to that of the Rust Enum.
It represents a piece of memory that can be occupied by one of several
distinct types, with a tag that indicates which type is in use.
The data layout of the union is similar to how one would implement
a tagged union in C:
#### Data layout:
```Rust
struct { union { ... } data; u8 tag; }
```
Except while Rust uses a 16-bit integer as a tag, this can use a smaller integer as the tag if there are less than 255 types.
See here [How Rust implements tagged unions](https://patshaughnessy.net/2018/3/15/how-rust-implements-tagged-unions).

The tag is an index value representing the *Nth* type in use, in the range [0, N]. It is not allowed to initialize a Union with no type in use.
# Initialization

A Union can be implicitly constructed from any datatype that is convertible to one of the datatypes it can store. There are three
levels of precedence, or matching, that determine which union member will be initialized from a provided datatype.

### Strong matching:
Obviously, values that are exactly the same type as one of the union's datatypes 

```C++
Union<int, float> x = 0.0f;
// Stores a float, because float was the first type which could be 
// explicitly initialized from 0.0f.
```
### Medium matching:
In cases where the datatype initializing the union is not the same as any of the Union's datatypes, some special rules are implemented to
make initialization still possible for some types.
```C++
Union<int, float> y = 0.0;
// Stores a float, because of special rule favoring float to be initialized from double, if the Union does not store a double.
    
Union<float, int> z = false;
// Stores an int. 
// Neither float nor int are the same type as bool, 
// but both float and int can be implicitly initialized from false. 
// However, medium matching favors int to be initialized from false.

```
### Weak matching:
In cases when the provided datatype is not the same as any of the Union's datatypes, but one or more of the Union's datatypes can be implicitly initialized from the provided values, and there is no medium
match, then the first type (according to the order of template arguments to the Union class) which can be implicitly initialized from
the value is chosen.
```C++
Union<int*, char*, void*> q = nullptr;
// Stores int* because all types can be implicitly initialized from
// nullptr, and int* is the first one.
```
### A Side Note:
The nuances of C++ types make it possible to do some interesting things,
like make a union that stores pointers but specifically captures null.
```C++
Union<int*, void*, char*, std::nullptr_t> q = nullptr;
```


# Usage