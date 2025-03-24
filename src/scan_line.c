#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scan_line.h"


// Create and return new token list
// Since a new list should always start with the same initial values,
// a constructor functions makes sense here
TokenList make_token_list() {
    TokenList list = {
        .tokens = NULL,
        .capacity = 0,
        .used = 0,
    };

    return list;
}

// Add a new token to the list
int add_token(TokenList *list, Token token) {
    long capacity = list -> capacity;
    long used     = list -> used;
    Token *temp;

    // If the list is full...
    if (capacity >= used) {
        // If the list is empty, set the capacity to default (16)
        // Otherwise, double the capacity
        list -> capacity = (capacity == 0) ? 16 : capacity * 2;

        // If realloc() fails, the original block is not freed
        // To prevent memory leaks, we must first verify its success
        // before reassigning the original pointer
        temp = (Token*)realloc(list -> tokens, list -> capacity * sizeof(Token));

        if (temp == NULL) {
            return 1;
        }

        list -> tokens = temp;
    }

    list -> tokens[list -> used] = token;
    list -> used++;

    return 0;
}

int scan_line(char *line, int line_number, TokenList *list) {
    int result = 0; // Return value; set by errors
    char *current = line;
    Token token;
    TokenType type;

    while (*current != 0) {
        // Each loop, we reset the token to a default state,
        // its correct state before adding it to the list
        token.type = DISCARD;

        switch (*current) {
            case '(':
                token.type = LPAREN;
                break;

            case ')':
                token.type = RPAREN;
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

            case ' ':
                break;

            case '\t':
                break;

            case '\n':
                // FIXME newline logic
                break;

            // If the character does not match any token, it is invalid
            // Continue scanning to catch as many invalid tokens as possible
            // rather than exiting immediately
            default:
                fprintf(stderr, "Error: Invalid token '%c' on line %d\n", *current, line_number);
                result = 1;
                break;
        }

        current++;

        if (token.type == DISCARD)
            continue;

        if (add_token(list, token) == 1) {
            fprintf(stderr, "Error scanning line %d:\n"
                    "Could not add token '%c' to token list\n"
                    "This is not your fault, it's mine. Oops.\n", line_number, *current);
            return 1;
        }
    }

    return result;
}
