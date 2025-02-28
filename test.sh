#!/bin/bash
#
# Run basic tests on the interpreter.
# For larger projects, I imagine a more robust approach would be needed. For a
# relatively small project, this is more than sufficient, and far simpler than a
# full unit testing framework.
#
EXIT_SUCCESS=0
EXIT_FAILURE=1
EXIT_INVALID_OPTION=2
EXIT_NO_PATH=3
EXIT_TOO_MANY_PATHS=4

# Call as: assert <case> <expected result>
assert () {
    if [[ $2 != $? ]]; then
        echo "Assertion failed: Case $1"
        echo "Expected result: $2"
        echo "Actual result: $?"
        exit 1
    fi
}

make > /dev/null 2>&1


# -----------------------
# |                     |
# |     ARG PARSING     |
# |                     |
# -----------------------

# No path or arguments
./main > /dev/null 2>&1
assert "./main" $EXIT_NO_PATH

# Only help flag
./main -h > /dev/null 2>&1
assert "./main -h" $EXIT_SUCCESS

# Invalid flag
./main -j > /dev/null 2>&1
assert "./main -j" $EXIT_INVALID_OPTION

# Help flag with invalid flag
./main -hj > /dev/null 2>&1
assert "./main -hj" $EXIT_INVALID_OPTION



echo "All tests passing."
