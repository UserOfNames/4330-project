#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_while.h"
#include "../../../src/lib/tokenlib.h"
#include "../../../src/lib/parse/while.h"
#include "../../../src/lib/parse/variables.h"
#include "../../../src/lib/parse/parselib.h"

int test_parse_while() {
    Variable *table = NULL;
    int result;

    // Failure: Bad condition
    Token while1[] = {
        make_token(WHILE),
        make_string_token("bad"),
        make_token(SEMICOLON),
        make_token(LSQUARE),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(RSQUARE),
        make_token(ENDPOINT),
    };
    _IP = while1;
    result = parse_while(&table);
    assert(result == EXIT_FAILURE);

    // Failure: Unterminated expression
    Token while2[] = {
        make_token(WHILE),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(LSQUARE),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(RSQUARE),
        make_token(ENDPOINT),
    };
    _IP = while2;
    result = parse_while(&table);
    assert(result == EXIT_FAILURE);

    // Failure: No '{'
    Token while3[] = {
        make_token(WHILE),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(RSQUARE),
        make_token(ENDPOINT),
    };
    _IP = while3;
    result = parse_while(&table);
    assert(result == EXIT_FAILURE);

    // Immediate false
    Token while4[] = {
        make_token(WHILE),
        make_number_token(3.0),
        make_token(GT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LSQUARE),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(SEMICOLON),
        make_token(RSQUARE),
        make_token(ENDPOINT),
    };
    _IP = while4;
    result = parse_while(&table);
    assert(result == EXIT_SUCCESS);

    // True, run three times
    Token while5[] = {
        make_token(WHILE),
        make_identifier_token("_var5"),
        make_token(LT),
        make_number_token(3.0),
        make_token(SEMICOLON),
        make_token(LSQUARE),
        make_token(PRINT),
        make_string_token("SHOULDPRINT3TIMES"),
        make_token(SEMICOLON),
        make_identifier_token("_var5"),
        make_token(EQ),
        make_identifier_token("_var5"),
        make_token(PLUS),
        make_number_token(1.0),
        make_token(SEMICOLON),
        make_token(RSQUARE),
        make_token(ENDPOINT),
    };
    set_variable(&table, "_var5", make_number_token(0.0));
    _IP = while5;
    result = parse_while(&table);
    assert(result == EXIT_SUCCESS);
    printf("-------------------\n");

    // Failure: No }, false condition
    Token while6[] = {
        make_token(WHILE),
        make_number_token(3.0),
        make_token(GT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LSQUARE),
        make_token(PRINT),
        make_string_token("SHOULDNOTPRINT"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = while6;
    result = parse_while(&table);
    assert(result == EXIT_FAILURE);

    // Failure: No }, true condition
    Token while7[] = {
        make_token(WHILE),
        make_number_token(3.0),
        make_token(LT),
        make_number_token(4.0),
        make_token(SEMICOLON),
        make_token(LSQUARE),
        make_token(PRINT),
        make_string_token("SHOULDPRINTDESPITEERROR"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = while7;
    result = parse_while(&table);
    assert(result == EXIT_FAILURE);

    destroy_variables(&table);
    return EXIT_SUCCESS;
}
