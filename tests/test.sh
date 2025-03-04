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
EXIT_INVALID_PATH=5

# Call as: assert <case> <expected result>
assert () {
    res=$?
    if [[ $2 != $res ]]; then
        echo "Assertion failed: Case $1"
        echo "Expected result: $2"
        echo "Actual result: $res"
        exit 1
    fi
}

alias main="bin/main"

make > /dev/null 2>&1


# -----------------------
# |                     |
# |     ARG PARSING     |
# |                     |
# -----------------------
# No path or flags
main > /dev/null 2>&1
assert "./main" $EXIT_NO_PATH

# Quiet flag with no path
main -q 2> /dev/null
assert "./main -q" $EXIT_NO_PATH

# Only help flag
main -h > /dev/null 2>&1
assert "./main -h" $EXIT_SUCCESS

# Invalid flag
main -j > /dev/null 2>&1
assert "./main -j" $EXIT_INVALID_OPTION

# Help flag with invalid flag
main -hj > /dev/null 2>&1
assert "./main -hj" $EXIT_INVALID_OPTION

# Quiet flag with invalid flag
main -qj 2> /dev/null
assert "./main -qj" $EXIT_INVALID_OPTION


# ------------------------
# |                      |
# |     OPENING FILE     |
# |                      |
# ------------------------
# Invalid file
main THERESNOWAYYOUMADEAFILENAMEDTHIS1234 > /dev/null 2>&1
assert "./main THERESNOWAYYOUMADEAFILENAMEDTHIS1234" $EXIT_INVALID_PATH

# Valid file, empty
touch IJUSTMADEAFILENAMEDTHISHOPEFULLYYOUDIDNT1234
main IJUSTMADEAFILENAMEDTHISHOPEFULLYYOUDIDNT1234 > /dev/null 2>&1
assert "./main IJUSTMADEAFILENAMEDTHISHOPEFULLYYOUDIDNT1234" $EXIT_SUCCESS
rm IJUSTMADEAFILENAMEDTHISHOPEFULLYYOUDIDNT1234


echo "All tests passing."
