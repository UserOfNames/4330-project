#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_ifelse.h"
#include "../../../src/lib/tokenlib.h"
#include "../../../src/lib/parse/ifelse.h"
#include "../../../src/lib/parse/variables.h"
#include "../../../src/lib/parse/parselib.h"

int test_parse_ifelse() {
    Variable *table = NULL;
    int result;

    // Failure: Bad condition
    Token if1[] = {
        make_token(IF),
        make_string_token("bad"),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(RCURLY),
        make_token(ENDPOINT),
    };
    _IP = if1;
    result = parse_ifelse(&table);
    assert(result == EXIT_FAILURE);

    // Failure: Unterminated expression
    Token if2[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("HERE"),
        make_token(RCURLY),
        make_token(ENDPOINT),
    };
    _IP = if2;
    result = parse_ifelse(&table);
    assert(result == EXIT_FAILURE);

    // Failure: No '{'
    Token if3[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ENDPOINT),
    };
    _IP = if3;
    result = parse_ifelse(&table);
    assert(result == EXIT_FAILURE);

    // True block without else
    Token if4[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDPRINT1"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ENDPOINT),
    };
    _IP = if4;
    result = parse_ifelse(&table);
    assert(result == EXIT_SUCCESS);

    // False block without else
    Token if5[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(GT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ENDPOINT),
    };
    _IP = if5;
    result = parse_ifelse(&table);
    assert(result == EXIT_SUCCESS);

    // Failure: No '{' after ELSE
    Token if6[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDPRINT2"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ELSE),
        make_token(PRINT),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ENDPOINT),
    };
    _IP = if6;
    result = parse_ifelse(&table);
    assert(result == EXIT_FAILURE);

    // True block with else
    Token if7[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDPRINT3"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ELSE),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ENDPOINT),
    };
    _IP = if7;
    result = parse_ifelse(&table);
    assert(result == EXIT_SUCCESS);

    // False block with else
    Token if8[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(GT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ELSE),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDPRINT4"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ENDPOINT),
    };
    _IP = if8;
    result = parse_ifelse(&table);
    assert(result == EXIT_SUCCESS);

    // If false else if true
    Token if9[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(GT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ELSE),
        make_token(LCURLY),
        make_token(IF),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDPRINT5"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(RCURLY),
        make_token(ENDPOINT),
    };
    _IP = if9;
    result = parse_ifelse(&table);
    assert(result == EXIT_SUCCESS);

    // No '}' on if
    Token if10[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDPRINTDESPITEERROR1"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = if10;
    result = parse_ifelse(&table);
    assert(result == EXIT_FAILURE);

    // No '}' on else; true condition
    Token if11[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDPRINTDESPITEERROR2"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ELSE),
        make_token(LCURLY),
        make_token(ENDPOINT),
    };
    _IP = if11;
    result = parse_ifelse(&table);
    assert(result == EXIT_FAILURE);

    // No '}' on else; false condition
    Token if12[] = {
        make_token(IF),
        make_number_token(3.0),
        make_token(GT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LCURLY),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(SEMICOLON),
        make_token(RCURLY),
        make_token(ELSE),
        make_token(LCURLY),
        make_token(ENDPOINT),
    };
    _IP = if12;
    result = parse_ifelse(&table);
    assert(result == EXIT_FAILURE);

    return EXIT_SUCCESS;
}
