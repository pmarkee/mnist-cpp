# Style guide

## General

* Soft line length limit is 100 characters. Avoid longer lines than this, but they can stay if it makes most sense that way.
* Hard line length limit is 120 characters.

## Naming conventions

* Class names are always `UpperCamelCase`.
* Namespace are `Uppercase`. (NOT camel!)
* `private` and `protected` members that are not exposed through getter-setters are `lowerCamelCase`.
* `private` and `protected` members that are exposed through getter-setters are `lowerCamelCase_`.
* Local variables are `snake_case`.
* Function and variables NOT exposed outside of compilation unit are `snake_case`.

## Classes

* Constructors should use initializer lists whenever possible. Initializer list items are preceded by a newline and an indentation, as shown in the example below.
* No public member variables. Any field that needs to be accessed must have a `public` getter.
* All functions, including getter-setters, must be declared in the header and defined in the corresponding `.cpp` file.
* All getters must use the `const` qualifier.
* Member variables should not be adjusted from outside the class, not even by setters. This is not a hard rule, it can be broken if absolutely needed.
* Generally avoid exposing pointers by getters. If it is needed, make both the pointer and the pointed value const.
* Getters should not have `get` in their name, just the name of the exposed field, without the `_` at the end.
* Always use the `this` keyword when referring to a member of the object.

Example:
```cpp
// a.h
class A {
private:
    int someValue_;
    int* intPtr_;
public:
    A(int someValue_, int* intPtr_);

    int someValue_() const; // THIS IS CORRECT.
    int getSomeValue();     // THIS IS NOT.

    const int* const intPtr() const; // THIS IS CORRECT.
    int* intPtr() const;             // THIS IS NOT.
}

// a.cpp
#include "a.h"

A::A(int someValue_, int* intPtr_)
    : someValue_(someValue_)
    , intPtr_(intPtr_)
{
}

int A::someValue() const
{
    return this->someValue_;
}

const int* const intPtr() const
{
    return this->intPtr_;
}
```

## Blocks

* Opening branckets should always be preceded and followed by a new line.

## Functions

* If a function is exposed to other compilation units, it should be in a class or a namespace, except operator overloads.
* Primitive values should be passed by value (with few exceptions).
* Objects should be passed by const reference whenever possible (an exception is when passing an `std::stream`).
* Mathematical functions that represent derivatives begin with a `d_` prefix.

## Memory

* Use as few dynamic allocations as possible. When using them, use `new`, `delete` and `delete[]` instead of `malloc() - free()`.
