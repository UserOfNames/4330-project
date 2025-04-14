#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../utils.h"
#include "../../../src/lib/parse/parselib.h"
#include "../../../src/lib/parse/assignment.h"
#include "test_assignment.h"

int test_parse_assignment() {
    Variable *table = NULL;
    Token expected, *result;

    // Valid assignment, literal bool
    Token assign1[] = {
        make_identifier_token("_ident1"),
        make_token(EQ),
        make_token(TRUE),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = assign1;
    expected = make_token(TRUE);
    assert(parse_assignment(&table) == EXIT_SUCCESS);
    result = get_variable(&table, "_ident1");
    assert(token_eq(*result, expected));

    // Valid assignment, literal number
    Token assign2[] = {
        make_identifier_token("_ident2"),
        make_token(EQ),
        make_number_token(9.9),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = assign2;
    expected = make_number_token(9.9);
    assert(parse_assignment(&table) == EXIT_SUCCESS);
    result = get_variable(&table, "_ident2");
    assert(token_eq(*result, expected));

    // Valid assignment, numeric expression
    Token assign3[] = {
        make_identifier_token("_ident3"),
        make_token(EQ),
        make_number_token(2.0),
        make_token(PLUS),
        make_token(LPAREN),
        make_number_token(3.0),
        make_token(SLASH),
        make_number_token(2.0),
        make_token(RPAREN),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = assign3;
    expected = make_number_token(2.0 + (3.0 / 2.0));
    assert(parse_assignment(&table) == EXIT_SUCCESS);
    result = get_variable(&table, "_ident3");
    assert(token_eq(*result, expected));

    // Invalid assignment: empty
    Token assign4[] = {
        make_identifier_token("_ident4"),
        make_token(EQ),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = assign4;
    expected = make_token(DISCARD);
    assert(parse_assignment(&table) == EXIT_FAILURE);
    result = get_variable(&table, "_ident4");
    assert(result == NULL);

    // Loose identifier
    Token assign5[] = {
        make_identifier_token("_ident5"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = assign5;
    expected = make_token(DISCARD);
    assert(parse_assignment(&table) == EXIT_FAILURE);
    result = get_variable(&table, "_ident5");
    assert(result == NULL);

    // Invalid assignment: Bad token
    Token assign6[] = {
        make_identifier_token("_ident6"),
        make_token(LPAREN),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = assign6;
    expected = make_token(DISCARD);
    assert(parse_assignment(&table) == EXIT_FAILURE);
    result = get_variable(&table, "_ident6");
    assert(result == NULL);

    // Assignment from identifier: valid
    Token assign7[] = {
        make_identifier_token("_ident7"),
        make_token(EQ),
        make_identifier_token("test7"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    set_variable(&table, "test7", make_number_token(3.0));
    _IP = assign7;
    expected = make_number_token(3.0);
    assert(parse_assignment(&table) == EXIT_SUCCESS);
    result = get_variable(&table, "_ident7");
    assert(token_eq(expected, *result));

    // Assignment from identifier: invalid name
    Token assign8[] = {
        make_identifier_token("_ident8"),
        make_token(EQ),
        make_identifier_token("nothere"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = assign8;
    expected = make_token(DISCARD);
    assert(parse_assignment(&table) == EXIT_FAILURE);
    result = get_variable(&table, "_ident8");
    assert(result == NULL);

    // Assignment from identifier: invalid type
    Token assign9[] = {
        make_identifier_token("_ident9"),
        make_token(EQ),
        make_identifier_token("badtype"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    set_variable(&table, "badtype", make_token(LPAREN));
    _IP = assign9;
    expected = make_token(DISCARD);
    assert(parse_assignment(&table) == EXIT_FAILURE);
    result = get_variable(&table, "_ident9");
    assert(result == NULL);

    // Unterminated
    Token assign10[] = {
        make_identifier_token("_ident10"),
        make_token(EQ),
        make_string_token("unterminated"),
        make_token(ENDPOINT),
    };
    _IP = assign10;
    expected = make_token(DISCARD);
    assert(parse_assignment(&table) == EXIT_FAILURE);
    result = get_variable(&table, "_ident10");
    assert(result == NULL);

    destroy_variables(&table);
    return EXIT_SUCCESS;
}
