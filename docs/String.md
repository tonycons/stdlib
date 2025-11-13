## A Design challenge: Supporting both procedural and functional programming

There is this divide between how some languages treat strings.
Some, like C++, allow the itself string to be modified.
Others like Python and Java, have immutable strings, where each modification done to
a string actually returns a new string. This has its advantages when it comes to functional programming, However, only allowing immutable strings has some drawback.

I've chosen a hybrid approach here.

* mutable String: (String&) behaves like a Java StringBuilder. Functions like replace, insert, etc. will modify the string in-place.
* immutable String (String const&) behaves like a Java String. Any modifications 
done to the string return a new string.

In my opinion, this is a more creative approach than what most C++ does. Typically, const just means "you're not allowed to use certain member functions"

### Examples:

<code>String&</code>, of course, is a mutable string.

```C++
String x = ...;
x.replace("foo", "bar"); // does what you think.
```
For an immutable string, functions that modify a string instead
return a copy with the modifications applied:

```C++
auto bar(String const& x) {
   return x.replace("foo", "bar"); // Returns a new string.
}
```
To avoid accidentally thinking the string is mutable and writing the wrong code, there is a compiler
warning if you don't use the return value here.
```C++
auto bar(String const& x) {
   x.replace("foo", "bar"); // Warning: "Immutable String cannot modify itself, instead returns a copy"
}
```

The same is true if the String is an L-Value or "immediate value," calling
a modification function on it will return a "new string", but the code is actually optimized to avoid unnecessary copies. It'll make more sense with this example:

```C++
auto s = String("abab").replace("a", "b").replace("bb", "c").erase(1, 1);
```
Here, the "abab" string is actually modified in-place 3 times rather than
a copy being made every time. Similarly, if any String along the "chain of
modification" is an L-value, it will then be modified in place, as we can
see here:

```C++
auto bar(String const& x) {
   return x.replace("a", "b").replace("cb", "c").insert(0, "d");
}
```
The contents of "x" are only copied for the first modification, afterwards,
the new string is repeatedly modified in-place until it is returned.

Thus, the performance of this "functional programming" looking code isn't
below anything from the C++ standard library.

The same behavior differences between mutable and immutable structures applies to others.



    /*
     some implementation notes-

     #1 =====================

        In general, when creating a new ResizableArray with a provided data pointer,
        we would have to copy the data onto our array rather than just set the array's buffer pointer to the data
        pointer. why? because the array might outlive the data pointer. consider the following:
            auto bar()
            {
                char hello[5] = {'H','e','l','l','o',0};
                return String(hello);
            }
        If we initialized the String by just setting its pointer to the data, the pointer would become invalid after
        the function returns. This means creating the string from a char pointer would require copying the chars
        But we shouldn't have to do that when creating a string from a string literal.

            String("hello");  // would be inefficient to copy the data

        You might think, "Well, we can overload the constructor for a string literal, like this:"
                template<unsigned N>
                String(char const(&str)[X]) { ... }    // #1 for string literals

                String(char const* str) { ... }  // #2 for non string literals

        Well, a string whose size is known at compile time is not necessarily a string literal.
            auto bar()
            {
                char hello[5] = {'H','e','l','l','o',0};
                return String(hello);
            }
        In this example, constructor #1 would be called. This is a problem.
        What we need is a way to just "know" if a pointer points to data on the stack, or to constant data...
        Meet  "  Ptr::isRomData()  "!  If it returns true, we know the pointer will be valid throughout the WHOLE
        duration of the program. So the constructor does not have to copy anything onto the array.

        One issue - if we are then going to modify elements in the array, and the array points to static data..
        That is a problem. THEN we have to create a copy of the data which is modifiable.

     #2 =====================

        To solve this problem, we will add a member variable called _needheap.
        When _needheap is true, it indicates that before any operation that could potentially modify the array data,
        a copy of the array data must be created on the heap. For example:

         String x = "Hello";     // _needheap is true:   nothing allocated on the heap, x is basically a const char*

         x[0] = "Y";        // User wants to modify string, so we put the string on the heap in order to modify it.
                            // now _needheap is false
    */