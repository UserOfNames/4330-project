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

    // Negation, singular
    Token expr10[] = {
        make_token(NEGATE),
        make_number_token(4.0),
    };
    _length = (sizeof(expr10) / sizeof(expr10[0]));
    _IP = expr10;
    expected = make_number_token(-4);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Negation, double
    Token expr11[] = {
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(4.0),
    };
    _length = (sizeof(expr11) / sizeof(expr11[0]));
    _IP = expr11;
    expected = make_number_token(4);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Negation, 5x
    Token expr12[] = {
        make_token(NEGATE),
        make_token(NEGATE),
        make_token(NEGATE),
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(4.0),
    };
    _length = (sizeof(expr12) / sizeof(expr12[0]));
    _IP = expr12;
    expected = make_number_token(-4);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Negation of 0
    Token expr13[] = {
        make_token(NEGATE),
        make_number_token(0.0),
    };
    _length = (sizeof(expr13) / sizeof(expr13[0]));
    _IP = expr13;
    expected = make_number_token(0);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Negation of a negative
    Token expr14[] = {
        make_token(NEGATE),
        make_number_token(-4.0),
    };
    _length = (sizeof(expr14) / sizeof(expr14[0]));
    _IP = expr14;
    expected = make_number_token(4.0);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Double negation of a negative
    Token expr15[] = {
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(-4.0),
    };
    _length = (sizeof(expr15) / sizeof(expr15[0]));
    _IP = expr15;
    expected = make_number_token(-4.0);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Logical negation, singular, false
    Token expr16[] = {
        make_token(BANG),
        make_token(FALSE),
    };
    _length = (sizeof(expr16) / sizeof(expr16[0]));
    _IP = expr16;
    expected = make_token(TRUE);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Logical negation, singular, true
    Token expr17[] = {
        make_token(BANG),
        make_token(TRUE),
    };
    _length = (sizeof(expr17) / sizeof(expr17[0]));
    _IP = expr17;
    expected = make_token(FALSE);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Logical negation, double, true
    Token expr18[] = {
        make_token(BANG),
        make_token(BANG),
        make_token(TRUE),
    };
    _length = (sizeof(expr18) / sizeof(expr18[0]));
    _IP = expr18;
    expected = make_token(TRUE);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Logical negation, double, true
    Token expr19[] = {
        make_token(BANG),
        make_token(BANG),
        make_token(FALSE),
    };
    _length = (sizeof(expr19) / sizeof(expr19[0]));
    _IP = expr19;
    expected = make_token(FALSE);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Complex arithmetic
    Token expr20[] = {
        make_token(NEGATE),
        make_number_token(2.0),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(LPAREN),
        make_number_token(4.3),
        make_token(SLASH),
        make_token(LPAREN),
        make_number_token(3.0),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(5.0),
        make_token(RPAREN),
        make_token(PLUS),
        make_number_token(3.0),
        make_token(RPAREN),
        make_token(STAR),
        make_number_token(7.0),
    };
    _length = (sizeof(expr20) / sizeof(expr20[0]));
    _IP = expr20;
    expected = make_number_token(-2+-(4.3/(3+-(-5))+3)*7);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Complex
    Token expr21[] = {
        make_token(NEGATE),
        make_number_token(2.0),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(LPAREN),
        make_number_token(4.3),
        make_token(SLASH),
        make_token(LPAREN),
        make_number_token(3.0),
        make_token(PLUS),
        make_number_token(5.0),
        make_token(RPAREN),
        make_token(PLUS),
        make_number_token(3.0),
        make_token(RPAREN),
        make_token(STAR),
        make_number_token(7.0),
        make_token(GT),
        make_number_token(5.0),
        make_token(MINUS),
        make_number_token(8.0),
        make_token(OR),
        make_token(NEGATE),
        make_number_token(38),
        make_token(MINUS),
        make_token(NEGATE),
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(2.0),
        make_token(STAR),
        make_number_token(4.0),
        make_token(EQ_EQ),
        make_number_token(100),
        make_token(AND),
        make_token(TRUE),
    };
    _length = (sizeof(expr21) / sizeof(expr21[0]));
    _IP = expr21;
    expected = make_token(FALSE);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Complex + variables
    Token expr22[] = {
        make_token(NEGATE),
        make_identifier_token("x"),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(LPAREN),
        make_number_token(4.3),
        make_token(SLASH),
        make_token(LPAREN),
        make_identifier_token("y"),
        make_token(PLUS),
        make_number_token(5.0),
        make_token(RPAREN),
        make_token(PLUS),
        make_number_token(3.0),
        make_token(RPAREN),
        make_token(STAR),
        make_number_token(7.0),
        make_token(GT),
        make_number_token(5.0),
        make_token(MINUS),
        make_number_token(8.0),
        make_token(OR),
        make_token(NEGATE),
        make_number_token(38),
        make_token(MINUS),
        make_token(NEGATE),
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(2.0),
        make_token(STAR),
        make_number_token(4.0),
        make_token(EQ_EQ),
        make_number_token(100),
        make_token(AND),
        make_identifier_token("t"),
    };
    set_variable(&table, "x", make_number_token(2.0));
    set_variable(&table, "y", make_number_token(3.0));
    set_variable(&table, "t", make_token(TRUE));
    _length = (sizeof(expr22) / sizeof(expr22[0]));
    _IP = expr22;
    expected = make_token(FALSE);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Complex arithmetic + variables
    Token expr23[] = {
        make_token(NEGATE),
        make_identifier_token("x"),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(LPAREN),
        make_number_token(4.3),
        make_token(SLASH),
        make_token(LPAREN),
        make_identifier_token("y"),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(5.0),
        make_token(RPAREN),
        make_token(PLUS),
        make_number_token(3.0),
        make_token(RPAREN),
        make_token(STAR),
        make_number_token(7.0),
    };
    _length = (sizeof(expr23) / sizeof(expr23[0]));
    _IP = expr23;
    expected = make_number_token(-2+-(4.3/(3+-(-5))+3)*7);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Complex arithmetic + invalid variable name
    Token expr24[] = {
        make_token(NEGATE),
        make_identifier_token("x"),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(LPAREN),
        make_number_token(4.3),
        make_token(SLASH),
        make_token(LPAREN),
        make_identifier_token("z"),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(5.0),
        make_token(RPAREN),
        make_token(PLUS),
        make_number_token(3.0),
        make_token(RPAREN),
        make_token(STAR),
        make_number_token(7.0),
    };
    _length = (sizeof(expr24) / sizeof(expr24[0]));
    _IP = expr24;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Complex arithmetic + invalid variable type
    Token expr25[] = {
        make_token(NEGATE),
        make_identifier_token("x"),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(LPAREN),
        make_number_token(4.3),
        make_token(SLASH),
        make_token(LPAREN),
        make_identifier_token("z"),
        make_token(PLUS),
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(5.0),
        make_token(RPAREN),
        make_token(PLUS),
        make_number_token(3.0),
        make_token(RPAREN),
        make_token(STAR),
        make_number_token(7.0),
    };
    set_variable(&table, "z", make_string_token("hi"));
    _length = (sizeof(expr25) / sizeof(expr25[0]));
    _IP = expr25;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Invalid type: arithmetic operator
    Token expr26[] = {
        make_token(TRUE),
        make_token(PLUS),
        make_number_token(4.0),
    };
    _length = (sizeof(expr26) / sizeof(expr26[0]));
    _IP = expr26;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Invalid type: boolean operator
    Token expr27[] = {
        make_number_token(3.0),
        make_token(AND),
        make_token(TRUE),
    };
    _length = (sizeof(expr27) / sizeof(expr27[0]));
    _IP = expr27;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Unexpected token in expresssion
    Token expr28[] = {
        make_number_token(4.2),
        make_token(AND),
        make_number_token(4.3),
        make_token(RCURLY),
    };
    _length = (sizeof(expr28) / sizeof(expr28[0]));
    _IP = expr28;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    destroy_variables(&table);
    return EXIT_SUCCESS;
}
