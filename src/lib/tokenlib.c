#include <stdio.h>
#include <stdlib.h>
#include "tokenlib.h"

// Generate a new token
Token make_token(TokenType type) {
    Token token = {
        .type = type,
        .literal.Number = 0,
    };

    return token;
}


// Generate a new token with a literal value
Token make_token_with_literal(TokenType type, Literal literal) {
    Token token = make_token(type);
    token.literal = literal;

    return token;
}


// Safely destroy a token, freeing associated memory if applicable
void destroy_token(Token *token) {
    switch(token -> type) {
        case STRING:
            free(token -> literal.String);
            token -> literal.Number = 0;
            token -> type = DISCARD;
            break;

        case NUMBER:
            token -> literal.Number = 0;
            token -> type = DISCARD;
            break;

        default:
            token -> type = DISCARD;
            break;
    }
}


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


void reset_token_list(TokenList *list) {
    long i;
    Token *tokens = list -> tokens;

    if (tokens != NULL) {
        // Walk the token list and destroy each token
        for (i=0; i<list->used; i++) {
            destroy_token((list -> tokens) + i);
        }

        free(tokens);
    }

    list -> tokens = NULL;
    list -> capacity = list -> used = 0;
}


// Add a new token to the list
int add_token(TokenList *list, Token token) {
    long capacity = list -> capacity;
    long used     = list -> used;
    Token *temp;

    // If the list is full...
    if (capacity <= used) {
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
