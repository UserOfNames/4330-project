#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "test_tokenlib.h"
#include "../../src/lib/tokenlib.h"

int test_make_token() {
    Token token = make_token(RPAREN);

    assert(token.type == RPAREN);

    return EXIT_SUCCESS;
}


int test_make_token_with_literal() {
    Token token = make_token_with_literal(RPAREN, (Literal){.String="hello world"});

    assert(token.type == RPAREN);
    assert(token.literal.String != NULL);
    assert(strcmp(token.literal.String, "hello world") == 0);

    return EXIT_SUCCESS;
}

int test_make_number_token() {
    Token token = make_number_token(-4.1);

    assert(token.type == NUMBER);
    assert(token.literal.Number == -4.1);

    return EXIT_SUCCESS;
}

int test_make_string_token() {
    Token token = make_string_token("hello world");

    assert(token.type == STRING);
    assert(strcmp(token.literal.String, "hello world") == 0);

    free(token.literal.String);
    return EXIT_SUCCESS;
}

int test_make_identifier_token() {
    Token token = make_identifier_token("_ident");

    assert(token.type == IDENTIFIER);
    assert(strcmp(token.literal.Name, "_ident") == 0);

    free(token.literal.Name);
    return EXIT_SUCCESS;
}

int test_destroy_token() {
    Token token = make_token(PLUS);
    destroy_token(&token);
    assert(token.type == DISCARD);
    assert(token.literal.Number == 0);

    char *s = (char*)malloc(13);
    strcpy(s, "Hello World!");
    token = make_token_with_literal(STRING, (Literal){.String=s});
    destroy_token(&token);
    assert(token.type == DISCARD);
    assert(token.literal.Number == 0);

    return EXIT_SUCCESS;
}


int test_make_token_list() {
    TokenList list = make_token_list();

    assert(list.tokens == NULL);
    assert(list.capacity == 0);
    assert(list.used == 0);

    return EXIT_SUCCESS;
}


int test_add_token() {
    TokenList list = make_token_list();

    // First token, list initalization
    add_token(&list, make_token(RPAREN));
    assert(list.tokens != NULL);
    assert(list.capacity == 16);
    assert(list.used == 1);

    // Second token, no list growth
    add_token(&list, make_token(LPAREN));
    assert(list.capacity == 16);
    assert(list.used == 2);

    // List should not grow until used exceeds capacity
    int i;
    for (i=0; i<14; i++) {
        add_token(&list, make_token(SEMICOLON));
    }
    assert(list.capacity == 16);
    assert(list.used == 16);

    // List capacity should double now
    add_token(&list, make_token(STAR));
    assert(list.capacity == 32);
    assert(list.used == 17);
    assert(list.tokens != NULL);

    return EXIT_SUCCESS;
}


int test_reset_token_list() {
    // Simple tokens
    TokenList list = make_token_list();
    add_token(&list, make_token(RPAREN));
    add_token(&list, make_token(LPAREN));
    add_token(&list, make_token(SEMICOLON));
    add_token(&list, make_token(STAR));
    reset_token_list(&list);

    assert(list.tokens == NULL);
    assert(list.capacity == 0);
    assert(list.used == 0);


    // Strings
    list = make_token_list();
    char *hw = malloc(12);
    hw[11] = 0;
    strcpy(hw, "hello world");

    char *gs = malloc(14);
    gs[13] = 0;
    strcpy(gs, "goodbye space");

    add_token(&list, make_token_with_literal(RPAREN, (Literal){.String=hw}));
    add_token(&list, make_token_with_literal(LPAREN, (Literal){.String=gs}));
    reset_token_list(&list);

    assert(list.tokens == NULL);

    return EXIT_SUCCESS;
}
