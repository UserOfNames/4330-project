#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "scan_line.h"
#include "constants.h"
#include "lib/tokenlib.h"


// Pointer to the current character being scanned It's useful to allow
// functions to mutate this, and passing a char** around with every call is
// much more annoying and much harder to read than just using a global.
char *current;

// When printing errors, it's good to include the line number containin the
// error. Again, to avoid having to pass the line number up every function
// call, we set it once and access it through this global.
int line_number;


// Check if the next character matches an expected character
// Used for two-character tokens
_Bool resolve_next(char expected, char *next) {
    if (*next == 0)
        return false;

    return *next == expected;
}


// Given a left pointer and right pointer to some part of a string, get a
// substring consisting of all characters from l to r
// Lower inclusive, upper exclusive, like a slice
char* get_substring(char *l, char *r) {
    size_t length = r - l;

    if (l > r || l == NULL || r == NULL)
        return NULL;

    char *result = malloc(length + 1); // +1 for null
    if (result == NULL) {
        fprintf(stderr, "Error in get_substring(): Could not allocate result buffer (line %d)\n", line_number);
        return NULL;
    }

    strncpy(result, l, length);
    result[length] = 0;

    return result;
}


// Check whether a given string is a keyword
TokenType resolve_word(char *str) {
    // Using a hashmap is more elegant, but
    // this is sufficient (and much simpler)
    // Unfortunately you can't switch against strings, so
    // it has to be conditional chains
    if (strcmp(str, "if") == 0) // strcmp() is safe as get_substring() null-terminates the result
        return IF;
    else if((strcmp(str, "else") == 0))
        return ELSE;
    else if((strcmp(str, "while") == 0))
        return WHILE;
    else if((strcmp(str, "false") == 0))
        return FALSE;
    else if((strcmp(str, "true") == 0))
        return TRUE;
    else if((strcmp(str, "and") == 0))
        return AND;
    else if((strcmp(str, "or") == 0))
        return OR;
    else if((strcmp(str, "let") == 0))
        return LET;
    else if((strcmp(str, "print") == 0))
        return PRINT;
    else if((strcmp(str, "none") == 0))
        return NONE;
    else
        return IDENTIFIER;
}


char* handle_string() {
    char *start = ++current;
    while (*current != '"') {
        if (*current == '\n' || *current == 0) {
            fprintf(stderr, "Error: Unterminated string on line %d\n", line_number);
            return NULL;
        }
        current++;
    }

    return get_substring(start, current);
}


double handle_number() {
    char *endp;
    double result = strtod(current, &endp);
    
    if ((result == 0 && current == endp) || endp == NULL) {
        fprintf(stderr, "Error: Could not parse number on line %d\n", line_number);
        return SCAN_LINE_FAILURE;
    }

    if (errno == ERANGE) {
        fprintf(stderr, "Error: Numeric overflow or underflow on line %d\n", line_number);
        return SCAN_LINE_FAILURE;
    }

    current = endp;
    return result;
}


TokenType handle_identifier() {
    char *start = current;

    while (isalpha(*current) || isdigit(*current) || *current == '_') {
        current++;
    }

    char *word = get_substring(start, current);
    return resolve_word(word);
}


int scan_line(char *line, int line_num, TokenList *list) {
    line_number = line_num; // Set global line number
    current = line;
    Token token;

    while (*current != 0) {
        // Each loop, we reset the token to a default state, then mutate it to
        // its correct state before adding it to the list
        // This is an optimization over using make_token()
        token.type = DISCARD;
        token.literal.Number = 0;

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

            case '{':
                token.type = LCURLY;
                break;

            case '}':
                token.type = RCURLY;
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
                token.type = resolve_next('=', ++current) ? NOT_EQ : BANG;
                break;

            case '=':
                token.type = resolve_next('=', ++current) ? EQ_EQ : EQ;
                break;

            case '>':
                token.type = resolve_next('=', ++current) ? GT_EQ : GT;
                break;

            case '<':
                token.type = resolve_next('=', ++current) ? LT_EQ : LT;
                break;

            case '/':
                token.type = SLASH;
                break;

            case '#':
                while (*current != '\n' && *current != 0)
                    current++;
                break;


            // Handle strings
            case '"':
                token.type = STRING;
                token.literal.String = handle_string();

                if (token.literal.String == NULL) {
                    fprintf(stderr, "Error parsing string on line %d\n", line_num);
                    return SCAN_LINE_ABORT;
                }

                break;


            case '\n':
                // FIXME newline logic
                break;

            // If the character does not match any other token:
            // First check if it's a number and handle accordingly.
            // Otherwise, the token is invalid. Stop scanning the line, but
            // continue scanning the file to catch other errors
            default:
                if (isdigit(*current)) {
                    token.type = NUMBER;
                    token.literal.Number = handle_number();
                    current--;
                    break;
                }

                else if (isalpha(*current) || *current == '_') {
                    token.type = handle_identifier();
                    break;
                }
                
                else {
                    fprintf(stderr, "Error: Invalid token '%c' on line %d\n", *current, line_num);
                    return SCAN_LINE_FAILURE;
                }
        }
        // End of switch statement

        current++;

        if (token.type == DISCARD)
            continue;

        if (add_token(list, token) == 1) {
            fprintf(stderr, "Error scanning line %d:\n"
                    "Could not add token '%c' to token list\n"
                    "This is not your fault, it's mine. Oops.\n", line_num, *current);
            return SCAN_LINE_ABORT;
        }
    }

    // fgets() always null-terminates the read buffer, so strlen() is safe here
    // If the last non-null character read is not a newline, the read line was
    // too long
    if (current[-1] != '\n') {
        fprintf(stderr, "Error: Line %d exceeds %d characters.\n", line_num, MAX_LINE_LENGTH);
        return SCAN_LINE_ABORT;
    }

    return SCAN_LINE_SUCCESS;
}
