Class project for CSC-4330 Programming Language Concepts, Spring 2025 semester,
Georgia State University, Professor Parag Tamhankar.

A basic interpreter for a small custom programming language.

Due to the time constraints and this being my first attempt to implement an
interpreter, the language is quite sparse, with only basic features, no
scope, and no ability to nest loops or conditionals.

Operators - (+, -, \*, /, ==, !=, <, <=, >, >=)

Types - Number (double), string, boolean, none

Variables - Global only and untyped. Assigned via <identifier> = <value>

Expressions - Arithmetic and boolean.

print <value> - Print an identifier or literal.

while <condition>; [] - Standard while loop. Nesting is NOT supported.

if <condition>; {} else {} - Standard conditionals. Nesting is NOT supported.

Example program:
var1 = 0;
var2 = 0;
while var1 < 3; [
    print var1;

    if var1 == 0; {
        print "var1 is now zero";
    } else {
        print "var1 is not one";
    }

    var1 = var1 + 1;
    var2 = 0;
]

Output:
0.000000
var1 is now zero
1.000000
var1 is not one
2.000000
var1 is not one

Building:

Simply use the provided Makefile. The program uses some GNU extensions, so I
cannot guarantee proper behavior if your system does not use glibc.

Usage:

"<executable name> -h" for info.
