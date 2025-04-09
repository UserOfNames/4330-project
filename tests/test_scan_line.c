#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "test_scan_line.h"
#include "../src/scan_line.h"

// Boolean strcmp() with null pointer handling
_Bool null_strcmp(char *l, char *r) {
    if (l == NULL) {
        if (r == NULL) return true;
        return false;
    }

    if (r == NULL)
        return false;

    if (!(strcmp(l, r) == 0))
        return false;

    return true;
}

// Verify that two tokens are equivalent
_Bool match_token(Token l, Token r) {
    if (l.type != r.type)
        return false;

    // We already verified that the types are equivalent,
    // so we only need to switch against one
    switch (l.type) {
        case STRING:
            return null_strcmp(l.literal.String, r.literal.String);

        case NUMBER:
            return l.literal.Number == r.literal.Number;

        case IDENTIFIER:
            return null_strcmp(l.literal.Name, r.literal.Name);

        default:
            return true;
    }
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


int test_get_substring() {
    char *one    = "";
    char *two    = "hello world";
    char *result;

    // Empty string
    result = get_substring(one, one);
    assert(result != NULL);
    assert(strcmp(result, "\0") == 0);

    // Simple string
    result = get_substring(two, two+5);
    assert(result != NULL);
    assert(strcmp(result, "hello\0") == 0);

    // l pointer greater (invalid case)
    result = get_substring(two+5, two-5);
    assert(result == NULL);

    // l == r in the middle of a string
    result = get_substring(two+3, two+3);
    assert(result != NULL);
    assert(strcmp(result, "\0") == 0);

    // NULL l
    result = get_substring(NULL, "");
    assert (result == NULL);

    // NULL r
    result = get_substring("", NULL);
    assert (result == NULL);

    // NULL l and r
    result = get_substring(NULL, NULL);
    assert (result == NULL);

    return EXIT_SUCCESS;
}


int test_scan_line() { 
    // Empty line
    TokenList list = make_token_list();
    int result = scan_line("", 1, &list);
    assert(result == SCAN_LINE_SUCCESS);


    // Invalid line
    reset_token_list(&list);
    result = scan_line("\\", 1, &list);
    assert(result == SCAN_LINE_FAILURE);


    // Single tokens
    reset_token_list(&list);
    Token expected1[] = {
        make_token(LPAREN),
        make_token(RPAREN),
        make_token(LCURLY),
        make_token(RCURLY),
        make_token(SEMICOLON),
        make_token(DOT),
        make_token(PLUS),
        make_token(MINUS),
        make_token(NEGATE),
        make_token(STAR),
        make_token(SLASH),
        make_token(BANG),
        make_token(EQ),
        make_token(GT),
        make_token(LT),
    };

    result = scan_line(" \t ( ) {} ; . + - ~ * / ! = > < \n", 1, &list);
    assert(result == SCAN_LINE_SUCCESS);
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
    assert(result == SCAN_LINE_SUCCESS);
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
        make_token(NOT_EQ), };

    result = scan_line("= != == ! = != ( != )\n", 1, &list);
    assert(result == SCAN_LINE_SUCCESS);
    assert(match_tl(&list, expected3, sizeof(expected3) / sizeof(expected3[0])));


    // Valid tokens and invalid tokens
    reset_token_list(&list);
    Token expected4[] = {
        make_token(LPAREN),
        make_token(RPAREN),
        make_token(NOT_EQ),
    };

    result = scan_line("()!=\\", 1, &list);
    assert(result == SCAN_LINE_FAILURE);
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
    assert(result == SCAN_LINE_SUCCESS);
    assert(match_tl(&list, expected5, sizeof(expected5) / sizeof(expected5[0])));


    // Valid string
    reset_token_list(&list);
    Token expected6[] = {
        make_token_with_literal(STRING, (Literal){.String="hello"}),
        make_token_with_literal(STRING, (Literal){.String="world"}),
    };

    result = scan_line("\"hello\" \"world\"\n", 1, &list);
    assert(result == SCAN_LINE_SUCCESS);
    assert(match_tl(&list, expected6, sizeof(expected6) / sizeof(expected6[0])));


    // Unterminated string
    reset_token_list(&list);
    Token expected7[] = {
        make_token_with_literal(STRING, (Literal){.String="hello"}),
    };

    result = scan_line("\"hello\" \"world\n", 1, &list);
    assert(result == SCAN_LINE_ABORT);
    assert(match_tl(&list, expected7, sizeof(expected7) / sizeof(expected7[0])));


    // Unterminated string, split by newline
    reset_token_list(&list);
    Token expected8[] = {
        make_token_with_literal(STRING, (Literal){.String="hello"}),
    };

    result = scan_line("\"hello\" \"world\n\"\n", 1, &list);
    assert(result == SCAN_LINE_ABORT);
    assert(match_tl(&list, expected8, sizeof(expected8) / sizeof(expected8[0])));


    // Unterminated string between valid tokens
    reset_token_list(&list);
    Token expected9[] = {
        make_token(LPAREN),
    };

    result = scan_line("(\"hello)\n", 1, &list);
    assert(result == SCAN_LINE_ABORT);
    assert(match_tl(&list, expected9, sizeof(expected9) / sizeof(expected9[0])));


    // Numbers
    reset_token_list(&list);
    Token expected10[] = {
        make_token_with_literal(NUMBER, (Literal){.Number=4}),
        make_token(DOT),
        make_token_with_literal(NUMBER, (Literal){.Number=4.3}),
        make_token(DOT),
        make_token_with_literal(NUMBER, (Literal){.Number=0}),
    };

    result = scan_line("4 . 4.3 . 0\n", 1, &list);
    assert(result == SCAN_LINE_SUCCESS);
    assert(match_tl(&list, expected10, sizeof(expected10) / sizeof(expected10[0])));


    // Identifiers and keywords
    reset_token_list(&list);
    Token expected11[] = {
        make_token(AND),
        make_token(IF),
        make_token_with_literal(IDENTIFIER, (Literal){.Name="iff"}),
        make_token(WHILE),
        make_token_with_literal(NUMBER, (Literal){.Number=4.2}),
        make_token_with_literal(IDENTIFIER, (Literal){.Name="_under"}),
        make_token(OR),
    };

    result = scan_line("and if iff while 4.2 _under or\n", 1, &list);
    assert(result == SCAN_LINE_SUCCESS);
    assert(match_tl(&list, expected11, sizeof(expected11) / sizeof(expected11[0])));

    // Everything
    reset_token_list(&list);
    Token expected12[] = {
        make_token(RPAREN),
        make_token(NOT_EQ),
        make_token(BANG),
        make_token(EQ),
        make_token(STAR),
        make_token(NOT_EQ),
        make_token_with_literal(NUMBER, (Literal){.Number=5}),
        make_token(GT),
        make_token(EQ),
        make_token_with_literal(IDENTIFIER, (Literal){.Name="_ident"}),
        make_token(OR),
        make_token_with_literal(IDENTIFIER, (Literal){.Name="ident"}),
        make_token(GT_EQ),
        make_token_with_literal(STRING, (Literal){.String="this is a string"}),
        make_token(DOT),
        make_token(PLUS),
        make_token(LT),
    };

    result = scan_line(")!= \t ! = * != # this is a comment\n 5 > = _ident or ident >= \"this is a string\" . + < \\ = <=\n", 1, &list);
    assert(result == SCAN_LINE_FAILURE);
    assert(match_tl(&list, expected12, sizeof(expected12) / sizeof(expected12[0])));

    // Complex arithmetic + boolean expression
    reset_token_list(&list);
    Token expected13[] = {
        make_token(MINUS),
        make_token_with_literal(NUMBER, (Literal){.Number=2}),
        make_token(PLUS),
        make_token(MINUS),
        make_token(LPAREN),
        make_token_with_literal(NUMBER, (Literal){.Number=4.3}),
        make_token(SLASH),
        make_token(LPAREN),
        make_token_with_literal(NUMBER, (Literal){.Number=3}),
        make_token(PLUS),
        make_token_with_literal(NUMBER, (Literal){.Number=5}),
        make_token(RPAREN),
        make_token(PLUS),
        make_token_with_literal(NUMBER, (Literal){.Number=3}),
        make_token(RPAREN),
        make_token(STAR),
        make_token_with_literal(NUMBER, (Literal){.Number=7}),
        make_token(GT),
        make_token_with_literal(NUMBER, (Literal){.Number=5}),
        make_token(MINUS),
        make_token_with_literal(NUMBER, (Literal){.Number=8}),
        make_token(OR),
        make_token(MINUS),
        make_token_with_literal(NUMBER, (Literal){.Number=38}),
        make_token(MINUS),
        make_token(MINUS),
        make_token(MINUS),
        make_token(MINUS),
        make_token_with_literal(NUMBER, (Literal){.Number=2}),
        make_token(STAR),
        make_token_with_literal(NUMBER, (Literal){.Number=4}),
        make_token(EQ_EQ),
        make_token_with_literal(NUMBER, (Literal){.Number=100}),
        make_token(AND),
        make_token(TRUE),
    };

    result = scan_line("-2 + -(4.3 / (3 + 5) + 3) * 7 > 5 - 8 or -38 - ---2 * 4 == 100 and true\n", 1, &list);
    assert(result == SCAN_LINE_SUCCESS);
    assert(match_tl(&list, expected13, sizeof(expected13) / sizeof(expected13[0])));


    // Line number
    reset_token_list(&list);
    result = scan_line("and or\n", 16, &list);
    assert(result == SCAN_LINE_SUCCESS);
    printf("HERE: %d\n", list.tokens[0].line);
    assert(list.tokens[0].line == 16);
    assert(list.tokens[1].line == 16);


    reset_token_list(&list);
    return EXIT_SUCCESS;
}
