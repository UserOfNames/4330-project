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
            return EXIT_FAILURE;
        }

        list -> tokens = temp;
    }

    list -> tokens[list -> used] = token;
    list -> used++;

    return EXIT_SUCCESS;
}

int scan_line(char *line, int line_number, TokenList *list) {
    char *current = line;
    Token token;

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
