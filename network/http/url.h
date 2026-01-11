#pragma once
#include <string>

/*
# explicit & implicit constructors

## Implicit constructor
URL(const std::string& raw);

- Allows automatic (implicit) type conversion.
- The compiler can convert a std::string or string literal into a URL object.

Example:
URL u = "http://example.com";   // allowed (implicit conversion)

Problem:
- Can create objects unintentionally.
- May cause confusing bugs in large codebases.


## Explicit constructor
explicit URL(const std::string& raw);

- Disables implicit type conversion.
- Object creation must be explicit and intentional.

Example:
URL u("http://example.com");    // allowed
URL u = "http://example.com";   // NOT allowed

Why use explicit:
- Prevents accidental conversions
- Makes code clearer and safer

Rule:
- Use `explicit` for all single-argument constructors unless implicit conversion is required.
*/


/*
std::string path() const;

This const is NOT about the return value.
It means: This method does not modify the object


`string path() const` == `string path(const URL* this)`
So inside path():

You cannot modify any member variables

You promise the compiler the object stays unchanged
*/


class URL {
public:
    // constructor
    URL(const std::string& raw);

    // getters functions
    std::string scheme() const;
    std::string host() const;
    std::string path() const;
    int port() const;

private:

    std::string raw_;
    std::string scheme_;
    std::string host_;
    std::string path_;
    int port_;

    void parse();
};
