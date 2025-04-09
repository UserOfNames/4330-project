#include <assert.h>
#include <stdio.h>

#include "test_parselib.h"
#include "../../../src/lib/parse/parselib.h"
#include "../../../src/lib/parse/variables.h"

_Bool token_eq(Token l, Token r) {
    if (l.type != r.type)
        return false;

    // We already verified the types are equivalent,
    // so we only need to check one
    if (l.type == TRUE || l.type == FALSE)
        return true;

    // Discard tokens indicate failure
    if (l.type == DISCARD)
        return true;

    if (l.type != NUMBER)
        return false; // Tokens should only be boolean or numeric here

    return (l.literal.Number == r.literal.Number);
}

int test_parse_expression() {
    Variable *table = NULL;
    Token expected, result;

    // Simple arithmetic
    Token expr1[] = { 
        make_number_token(2),
        make_token(PLUS),
        make_number_token(2),
        make_token(SEMICOLON),
    };
    _length = (sizeof(expr1) / sizeof(expr1[0]));
    _IP = expr1;
    expected = make_number_token(2.0 + 2.0);
    result = parse_expression(&table);
    assert(token_eq(result, expected));


    // Arithmetic, binary operators only
    Token expr2[] = { 
        make_number_token(2.0),
        make_token(PLUS),
        make_token(LPAREN),
        make_number_token(2.0),
        make_token(MINUS),
        make_token(LPAREN),
        make_number_token(8.0),
        make_token(MINUS),
        make_number_token(6.0),
        make_token(RPAREN),
        make_token(RPAREN),
        make_token(STAR),
        make_token(LPAREN),
        make_number_token(10.0),
        make_token(RPAREN),
        make_token(SLASH),
        make_number_token(2.0),
        make_token(SEMICOLON),
    };
    _length = (sizeof(expr2) / sizeof(expr2[0]));
    _IP = expr2;
    expected = make_number_token
        (2.0 + (2.0 - (8.0 - 6.0)) * (10.0) / 2);
    result = parse_expression(&table);
    assert(token_eq(result, expected));


    // Simple boolean
    Token expr3[] = {
        make_number_token(5),
        make_token(GT),
        make_number_token(2),
        make_token(SEMICOLON),
    };
    _length = (sizeof(expr3) / sizeof(expr3[0]));
    _IP = expr3;
    expected = make_token(TRUE);
    result = parse_expression(&table);

    assert(token_eq(result, expected));

    // Boolean, binary operators only
    Token expr4[] = {
        make_number_token(5.0),
        make_token(GT_EQ),
        make_number_token(5.2),
        make_token(OR),
        make_token(LPAREN),
        make_token(TRUE),
        make_token(AND),
        make_token(FALSE),
        make_token(OR),
        make_number_token(7.0),
        make_token(NOT_EQ),
        make_number_token(7.0),
        make_token(RPAREN),
        make_token(SEMICOLON),
    };
    _length = (sizeof(expr4) / sizeof(expr4[0]));
    _IP = expr4;
    expected = make_token(FALSE);
    result = parse_expression(&table);

    assert(token_eq(result, expected));

    // Only parentheses
    Token expr5[] = {
        make_token(LPAREN),
        make_token(LPAREN),
        make_token(RPAREN),
        make_token(LPAREN),
        make_token(RPAREN),
        make_token(RPAREN),
        make_token(SEMICOLON),
    };
    _length = (sizeof(expr5) / sizeof(expr5[0]));
    _IP = expr5;
    expected = make_token(DISCARD);
    result = parse_expression(&table);

    assert(token_eq(result, expected));

    // Empty expression
    Token expr6[] = {};
    _length = 0;
    _IP = expr6;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Initial parentheses
    Token expr7[] = {
        make_token(LPAREN),
        make_number_token(6.0),
        make_token(PLUS),
        make_number_token(7.5),
        make_token(RPAREN),
    };
    _length = (sizeof(expr7) / sizeof(expr7[0]));
    _IP = expr7;
    expected = make_number_token(6.0 + 7.5);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Mismatched parentheses, R
    Token expr8[] = {
        make_token(LPAREN),
        make_number_token(6.0),
        make_token(PLUS),
        make_number_token(7.5),
        make_token(RPAREN),
        make_token(RPAREN),
        make_token(SEMICOLON),
    };
    _length = (sizeof(expr8) / sizeof(expr8[0]));
    _IP = expr8;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Mismatched parentheses, L
    Token expr9[] = {
        make_token(LPAREN),
        make_token(LPAREN),
        make_number_token(6.0),
        make_token(PLUS),
        make_number_token(7.5),
        make_token(RPAREN),
        make_token(SEMICOLON),
    };
    _length = (sizeof(expr9) / sizeof(expr9[0]));
    _IP = expr9;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    Token expr10[] = {
        make_token(MINUS),
        make_number_token(4),
    };
    _length = (sizeof(expr10) / sizeof(expr10[0]));
    _IP = expr10;
    expected = make_number_token(-4);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // // Complex
    // Token expr5[] = {
    //     make_token(MINUS),
    //     make_token(2.0),
    //     make_token(PLUS),
    //     make_token(MINUS),
    //     make_token(LPAREN),
    //     make_number_token(4.3),
    //     make_token(SLASH),
    //     make_token(LPAREN),
    //     make_number_token(3.0),
    //     make_token(PLUS),
    //     make_number_token(5.0),
    //     make_token(RPAREN),
    //     make_token(PLUS),
    //     make_number_token(3.0),
    //     make_token(RPAREN),
    //     make_token(STAR),
    //     make_number_token(7.0),
    //     make_token(GT),
    //     make_number_token(5.0),
    //     make_token(MINUS),
    //     make_number_token(8.0),
    //     make_token(OR),
    //     make_token(MINUS),
    //     make_number_token(38),
    //     make_token(MINUS),
    //     make_token(MINUS),
    //     make_token(MINUS),
    //     make_token(MINUS),
    //     make_number_token(2.0),
    //     make_token(STAR),
    //     make_number_token(4.0),
    //     make_token(EQ_EQ),
    //     make_number_token(100),
    //     make_token(AND),
    //     make_token(TRUE),
    // };
    // _length = (sizeof(expr5) / sizeof(expr5[0]));
    // _IP = expr5;
    // expected = make_token(FALSE);
    // result = parse_expression(&table);
    //
    // assert(token_eq(result, expected));

    return EXIT_SUCCESS;
}
