# Style guide

## Naming conventions

* Class names are always `UpperCamelCase`.
* Namespace are `Uppercase`. (NOT camel!)
* `private` and `protected` members that are not exposed through getter-setters are `lowerCamelCase`.
* `private` and `protected` members that are exposed through getter-setters are `lowerCamelCase_`.
* Local variables are `snake_case`.

## Classes

* No public member variables. Any field that needs to be accessed must have a `public` getter.
* All getters must use the `const` qualifier.
* Member variables should not be adjusted from outside the class, not even by setters. This is not a hard rule, it can be broken if absolutely needed.
* Getters should not have `get` in their name, just the name of the exposed field, without the `_` at the end. Example:
```cpp
class A {
private:
    int someValue_;
public:
    int someValue_() const; // THIS IS CORRECT
    int getSomeValue();     // THIS IS NOT
}
```
* Always use the `this` keyword when referring to a member of the object.
* Never use getters from inside the class.

## Functions

* If a function is exposed to other compilation units, it should be in a class or a namespace, except operator overloads.
* Primitive values should be passed by value (with few exceptions).
* Objects should be passed by const reference whenever possible (an exception is when passing an `std::stream`).

## Memory

* Use as few dynamic allocations as possible. When using them, use `new`, `delete` and `delete[]` instead of `malloc() - free()`.
