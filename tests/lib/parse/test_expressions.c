#include <assert.h>
#include <stdio.h>

#include "../../utils.h"
#include "test_expressions.h"
#include <stdbool.h>
#include "../../../src/lib/parse/parselib.h"
#include "../../../src/lib/parse/expressions.h"
#include "../../../src/lib/parse/variables.h"

int test_parse_expression() {
    Variable *table = NULL;
    Token expected, result;

    // Simple arithmetic
    Token expr1[] = { 
        make_number_token(2),
        make_token(PLUS),
        make_number_token(2),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
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
        make_token(ENDPOINT),
    };
    
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
        make_token(ENDPOINT),
    };
    
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
        make_token(ENDPOINT),
    };
    
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
        make_token(ENDPOINT),
    };
    
    _IP = expr5;
    expected = make_token(DISCARD);
    result = parse_expression(&table);

    assert(token_eq(result, expected));

    // Empty expression
    Token expr6[] = {};
    
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
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
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
        make_token(ENDPOINT),
    };
    
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
        make_token(ENDPOINT),
    };
    
    _IP = expr9;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Negation, singular
    Token expr10[] = {
        make_token(NEGATE),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr10;
    expected = make_number_token(-4);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Negation, double
    Token expr11[] = {
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
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
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr12;
    expected = make_number_token(-4);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Negation of 0
    Token expr13[] = {
        make_token(NEGATE),
        make_number_token(0.0),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr13;
    expected = make_number_token(0);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Negation of a negative
    Token expr14[] = {
        make_token(NEGATE),
        make_number_token(-4.0),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr14;
    expected = make_number_token(4.0);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Double negation of a negative
    Token expr15[] = {
        make_token(NEGATE),
        make_token(NEGATE),
        make_number_token(-4.0),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr15;
    expected = make_number_token(-4.0);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Logical negation, singular, false
    Token expr16[] = {
        make_token(BANG),
        make_token(FALSE),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr16;
    expected = make_token(TRUE);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Logical negation, singular, true
    Token expr17[] = {
        make_token(BANG),
        make_token(TRUE),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr17;
    expected = make_token(FALSE);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Logical negation, double, true
    Token expr18[] = {
        make_token(BANG),
        make_token(BANG),
        make_token(TRUE),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr18;
    expected = make_token(TRUE);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Logical negation, double, true
    Token expr19[] = {
        make_token(BANG),
        make_token(BANG),
        make_token(FALSE),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
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
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
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
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
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
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    set_variable(&table, "x", make_number_token(2.0));
    set_variable(&table, "y", make_number_token(3.0));
    set_variable(&table, "t", make_token(TRUE));
    
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
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
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
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
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
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    set_variable(&table, "z", make_string_token("hi"));
    
    _IP = expr25;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Invalid type: arithmetic operator
    Token expr26[] = {
        make_token(TRUE),
        make_token(PLUS),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr26;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Invalid type: boolean operator
    Token expr27[] = {
        make_number_token(3.0),
        make_token(AND),
        make_token(TRUE),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
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
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr28;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Only one value
    Token expr29[] = {
        make_number_token(4.2),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr29;
    expected = make_number_token(4.2);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Only one variable
    Token expr30[] = {
        make_identifier_token("x"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    
    _IP = expr30;
    expected = make_number_token(2.0);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    // Unterminated
    Token expr31[] = {
        make_number_token(3.0),
        make_token(PLUS),
        make_number_token(5.0),
        make_token(ENDPOINT),
    };
    
    _IP = expr31;
    expected = make_token(DISCARD);
    result = parse_expression(&table);
    assert(token_eq(result, expected));

    destroy_variables(&table);
    return EXIT_SUCCESS;
}
