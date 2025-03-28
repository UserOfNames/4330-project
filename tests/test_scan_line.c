#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "test_scan_line.h"
#include "../src/scan_line.h"

// Verify that two tokens are equivalent
_Bool match_token(Token l, Token r) {
    if (l.type != r.type)
        return false;

    if (l.lexeme == NULL) {
        if (r.lexeme == NULL)
            return true;
        return false;
    }

    if (r.lexeme == NULL)
        return false;

    if (!(strcmp(l.lexeme, r.lexeme) == 0))
        return false;

    return true;
}

// Verify that a TokenList matches an expected output
_Bool match_tl(TokenList *list, Token *expected, int expected_length) {
    Token *ptr_l = list-> tokens;
    Token *ptr_e = expected;
    int loop_count = 0;

    while (ptr_l < (list-> tokens + list-> capacity) &&
           loop_count < expected_length) {
        if (!match_token(*ptr_l, *ptr_e)) {
            return false;
        }

        ptr_l++;
        ptr_e++;
        loop_count++;
    }

    return true;
}

int test_scan_line() { 
    // Empty line
    TokenList list = make_token_list();
    int result = scan_line("", 1, &list);
    assert(result == EXIT_SUCCESS);


    // Invalid line
    reset_token_list(&list);
    result = scan_line("ABCDE F G1 235", 1, &list);
    assert(result == EXIT_FAILURE);


    // Single tokens
    reset_token_list(&list);
    Token expected1[] = {
        make_token(LPAREN),
        make_token(RPAREN),
        make_token(DOT),
        make_token(PLUS),
        make_token(MINUS),
        make_token(STAR),
        make_token(SLASH),
        make_token(BANG),
        make_token(EQ),
        make_token(GT),
        make_token(LT),
    };

    result = scan_line(" \t ( ) . + - * / ! = > < \n", 1, &list);
    assert(result == EXIT_SUCCESS);
    assert(match_tl(&list, expected1, sizeof(expected1) / sizeof(expected1[0])));


    // Double tokens
    reset_token_list(&list);
    Token expected2[] = {
        make_token(NOT_EQ),
        make_token(EQ_EQ),
        make_token(GT_EQ),
        make_token(LT_EQ),
    };

    result = scan_line("!= == >= <=\n", 1, &list);
    assert(result == EXIT_SUCCESS);
    assert(match_tl(&list, expected2, sizeof(expected2) / sizeof(expected2[0])));


    // Double tokens and single tokens
    reset_token_list(&list);
    Token expected3[] = {
        make_token(EQ),
        make_token(NOT_EQ),
        make_token(EQ_EQ),
        make_token(BANG),
        make_token(EQ),
        make_token(NOT_EQ),
        make_token(LPAREN),
        make_token(NOT_EQ),
    };

    result = scan_line("= != == ! = != ( != )\n", 1, &list);
    assert(result == EXIT_SUCCESS);
    assert(match_tl(&list, expected3, sizeof(expected3) / sizeof(expected3[0])));


    // Valid tokens and invalid tokens
    reset_token_list(&list);
    Token expected4[] = {
        make_token(LPAREN),
        make_token(RPAREN),
        make_token(NOT_EQ),
    };

    result = scan_line("()!=A", 1, &list);
    assert(result == EXIT_FAILURE);
    assert(match_tl(&list, expected4, sizeof(expected4) / sizeof(expected4[0])));


    // Comment
    reset_token_list(&list);
    Token expected5[] = {
        make_token(LPAREN),
        make_token(RPAREN),
        make_token(LPAREN),
        make_token(RPAREN),
        make_token(PLUS),
        make_token(MINUS),
    };

    result = scan_line("() # this is a comment\n()+-#()+-\n", 1, &list);
    assert(result == EXIT_SUCCESS);
    assert(match_tl(&list, expected5, sizeof(expected5) / sizeof(expected5[0])));


    reset_token_list(&list);
    return EXIT_SUCCESS;
}
