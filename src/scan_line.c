#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "scan_line.h"
#include "constants.h"
#include "tokenlib.h"


// Check if the next character matches an expected character
// Used for two-character tokens
_Bool resolve(char expected, char *next) {
    if (*next == 0)
        return false;

    return *next == expected;
}


// Left pointer and right pointer to some part of a string, get a
// substring consisting of all characters from l to r
// Lower inclusive, upper exclusive, like a slice
// It is the responsibility of the caller to ensure l and r are valid pointers
char* get_substring(char *l, char *r) {
    size_t length = r - l;

    if (l > r || l == NULL || r == NULL)
        return NULL;

    char *result = malloc(length + 1); // +1 for null
    if (result == NULL) {
        fprintf(stderr, "Error in get_substring(): Could not allocate result buffer\n");
        return NULL;
    }

    strncpy(result, l, length);
    result[length] = 0;

    return result;
}


int scan_line(char *line, int line_number, TokenList *list) {
    char *current = line;
    Token token;

    // Some tokens take several characters. This is used when you need to point
    // to the start of a long token, e.g. string literals
    char *start = line;

    while (*current != 0) {
        // Each loop, we reset the token to a default state, then mutate it to
        // its correct state before adding it to the list
        // This is an optimization over using make_token()
        token.type   = DISCARD;
        token.lexeme = NULL;

        switch (*current) {
            case ' ':
                break;

            case '\t':
                break;

            case '(':
                token.type = LPAREN;
                break;

            case ')':
                token.type = RPAREN;
                break;

            case '.':
                token.type = DOT;
                break;

            case '+':
                token.type = PLUS;
                break;

            case '-':
                token.type = MINUS;
                break;

            case '*':
                token.type = STAR;
                break;

            case '!':
                token.type = resolve('=', ++current) ? NOT_EQ : BANG;
                break;

            case '=':
                token.type = resolve('=', ++current) ? EQ_EQ : EQ;
                break;

            case '>':
                token.type = resolve('=', ++current) ? GT_EQ : GT;
                break;

            case '<':
                token.type = resolve('=', ++current) ? LT_EQ : LT;
                break;

            case '/':
                token.type = SLASH;
                break;

            case '#':
                while (current[1] != '\n' && current[1] != 0)
                    current++;
                break;

            case '"':
                start = current++;
                while (*current != '"') {
                    if (*current == '\n' || *current == 0) {
                        fprintf(stderr, "Error: Unterminated string on line %d\n", line_number);
                        return SCAN_LINE_FAILURE;
                    }
                    current++;
                }

                token.type   = STRING;
                token.lexeme = get_substring(start+1, current);
                break;

            case '\n':
                // FIXME newline logic
                break;

            // If the character does not match any token, it is invalid
            // Stop scanning the line, but continue scanning the file
            // to catch other errors
            default:
                fprintf(stderr, "Error: Invalid token '%c' on line %d\n", *current, line_number);
                return SCAN_LINE_FAILURE;
        }

        current++;

        if (token.type == DISCARD)
            continue;

        if (add_token(list, token) == 1) {
            fprintf(stderr, "Error scanning line %d:\n"
                    "Could not add token '%c' to token list\n"
                    "This is not your fault, it's mine. Oops.\n", line_number, *current);
            return SCAN_LINE_ABORT;
        }
    }

    // fgets() always null-terminates the read buffer, so strlen() is safe here
    // If the last non-null character read is not a newline, the read line was
    // too long
    if (current[-1] != '\n') {
        fprintf(stderr, "Error: Line %d exceeds %d characters.\n", line_number, MAX_LINE_LENGTH);
        return SCAN_LINE_ABORT;
    }

    return SCAN_LINE_SUCCESS;
}
