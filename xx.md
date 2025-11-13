

References, copy constructor, copy assignment, move semantics, r-values, l-values... It seems 
antithetical to C++ as a high-level language that the programmer has to be thinking so in-depth
about "how are my objects being passed around in memory"?
Wasn't C simpler with just pointers and values? How can the "abstraction" of C++ be 8 times more complex? 

Now sure, all those "references" and "move semantics" enable you to have fancy syntax where you
can manipulate strings with arithmetic operators. But I'm hesitant to use these features in an
environment where the entire burden of memory and performance is placed on the programmer.

Or just by omitting an ampersand & in one of your function parameters (<code>const string</code> instead of <code>const string&</code>), you could accidentally
trigger a deep copy of 10 megabytes of data?

One of the key points of having operator overloading is to allow scientists and mathematicians to
translate their work into code, while preserving the mathematical syntax. It also lowers the
language barrier from math to code. However, it fails at this because it's so easy to write bad performing code if you're not familiar enough with C++. 
For example,
```C++
std::string s = "Hello";

// #1:
s += " World";  // Appends " World" to "Hello"
// #2:
s = s + " World";   // Mathematically, it's the same as #1 but less performant

```
Another common idiom:
```C++
std::string build_http_header() {
    std::string result;
    // Here we build the string according to some complex logic...
    return result;
}
std::string header = build_http_header();
```
What this does is create an instance of <code>std::string</code> on the stack, append some stuff to it, and then return it. Simple? But the compiler can't return it as a reference or pointer (because
it's a stack variable which will be invalid once the function returns).
So, it has to deep copy <code>result</code> into <code>header</code> Then destroy result. To avoid
this unnecessary overhead, different approaches like this have to be taken:
```C++
std::unique_ptr<std::string> build_http_header() {
    auto* result = new std::string();
    // Here we build the string according to some complex logic...
    return std::make_unique(result);
}
```
Is a heap allocation really necessary? How about this:
```C++
void build_http_header(std::string& result) {
    assert(result.empty()); // prevent non-empty string from being given
    // modify result...
}
std::string header;
build_http_header(header);
```
This is better, we don't force the string to be heap-allocated, but still a few problems. We don't create the string, so an edge case appears where the string is non-empty. It's not good if we are trying to follow functional programming principles (it's not a pure function). An alternative I've come up with is this:
```C++
std::string& build_http_header(std::string const& result = {}) {
    // modify result by using const_cast..
    return const_cast<std::string&>(result);
}
std::string& header = build_http_header();

// It would be as if we did this:
// std::string result;
// std::string& header = build_http_header(result); // header references 
```

My point is, if we're going to treat strings and other data structures like "primitive types" by giving them arithmetic operators and other bells and whistles, then it shouldn't be wrong to use them
like primitive types. I should be able to pass string to a function exactly the same as passing int. I shouldn't have to care about the performance implications of how I return a string from a function.




Suppose I have a class called <code>ImmutableString</code>, and one of its members is a pointer of the data. Normally, in a copy constructor, there would need to be a deep copy of the data from the other string. Since a shallow copy would just have this string pointing to the other string's data, that would mean when the other string is destroyed, this string is left with a dangling pointer.

However, if using garbage collection, <code>ImmutableString</code> does not require any deep copying,
or destructor to free the data. The data will simply be collected once no instances of <code>ImmutableString</code> reference it. It is now a ["trivial data type"](https://en.cppreference.com/w/cpp/language/classes.html#Trivial_class), which is more performant. It can be safely copied
using memcpy.

This changes common data structures into being *views into data* rather than *owning the data*. 