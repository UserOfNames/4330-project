# 4330-project
Class project for CSC-4330 Programming Language Concepts, Spring 2025 semester,
Georgia State University, Professor Parag Tamhankar.

A basic interpreter for a small custom programming language.

## Features
Due to time constraints and this being my first attempt at implementing an
interpreter, the language is quite sparse, with only basic features, no scopes,
no functions, and no ability to nest loops or conditionals.

- **Operators**: `+`, `-`, `*`, `/`, `==`, `!=`, `<`, `<=`, `>`, `>=`
- **Types**: Number (double), string, boolean, none
- **Variables**: Global only and untyped. Assigned via `<identifier> = <value>`
- **Expressions**: Arithmetic and boolean
- **Print**: `print <value>` - Prints an identifier or literal
- **While Loop**: `while <condition>; []` - Standard while loop. Nesting is NOT supported
- **Conditionals**: `if <condition>; {} else {}` - Standard conditionals. Nesting is NOT supported

## Example Program
```plaintext
var1 = 0;
var2 = 0;
while var1 < 3; [
    print var1;

    if var1 == 0; {
        print "var1 is now zero";
    } else {
        print "var1 is not zero";
    }

    var1 = var1 + 1;
    var2 = 0;
]
```
**Output:**
```plaintext
0
var1 is now zero
1
var1 is not zero
2
var1 is not zero
```

## Building
Use the provided Makefile. The program uses some GNU extensions, so compatibility is not guaranteed on systems without glibc.

## Usage
Run ```<executable name> -h``` for usage information.
