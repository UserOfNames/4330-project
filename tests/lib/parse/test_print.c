#include <assert.h>
#include <stdio.h>

#include "test_print.h"
#include "../../../src/lib/parse/print.h"
#include "../../../src/lib/parse/parselib.h"

int test_print_value() {
    Variable *table = NULL;
    int result;

    Token print1[] = {
        make_token(PRINT),
        make_token(TRUE),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = print1;
    // Since testing what we write to stdout directly would be kinda
    // tricky, manually compare the EXPECTED: output to the actual output
    printf("EXPECTED: True\nACTUAL: ");
    result = print_value(&table);
    assert(result == EXIT_SUCCESS);
    printf("\n");

    Token print2[] = {
        make_token(PRINT),
        make_token(FALSE),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = print2;
    printf("EXPECTED: False\nACTUAL: ");
    result = print_value(&table);
    assert(result == EXIT_SUCCESS);
    printf("\n");

    Token print3[] = {
        make_token(PRINT),
        make_token(NONE),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = print3;
    printf("EXPECTED: None\nACTUAL: ");
    result = print_value(&table);
    assert(result == EXIT_SUCCESS);
    printf("\n");

    Token print4[] = {
        make_token(PRINT),
        make_number_token(3.0),
        make_token(PLUS),
        make_number_token(5.2),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = print4;
    printf("EXPECTED: 8.2\nACTUAL: ");
    result = print_value(&table);
    assert(result == EXIT_SUCCESS);
    printf("\n");

    Token print5[] = {
        make_token(PRINT),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = print5;
    printf("EXPECTED: Error...\nACTUAL: ");
    fflush(stdout);
    result = print_value(&table);
    assert(result == EXIT_FAILURE);
    printf("\n");

    Token print6[] = {
        make_token(PRINT),
        make_identifier_token("_ident6"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    set_variable(&table, "_ident6", make_token(NONE));
    _IP = print6;
    printf("EXPECTED: None\nACTUAL: ");
    result = print_value(&table);
    assert(result == EXIT_SUCCESS);
    printf("\n");

    Token print7[] = {
        make_token(PRINT),
        make_identifier_token("_ident7"),
        make_token(PLUS),
        make_number_token(1.1),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    set_variable(&table, "_ident7", make_number_token(7.0));
    _IP = print7;
    printf("EXPECTED: 8.1\nACTUAL: ");
    result = print_value(&table);
    assert(result == EXIT_SUCCESS);
    printf("\n");

    Token print8[] = {
        make_token(PRINT),
        make_identifier_token("_ident8"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    set_variable(&table, "_ident8", make_string_token("string identifier"));
    _IP = print8;
    printf("EXPECTED: string identifier\nACTUAL: ");
    result = print_value(&table);
    assert(result == EXIT_SUCCESS);
    printf("\n");

    Token print9[] = {
        make_token(PRINT),
        make_string_token("string"),
        make_token(SEMICOLON),
        make_token(ENDPOINT),
    };
    _IP = print9;
    printf("EXPECTED: string\nACTUAL: ");
    result = print_value(&table);
    assert(result == EXIT_SUCCESS);
    printf("\n");
    assert(_IP -> type == SEMICOLON);

    // Unterminated: literal
    Token print10[] = {
        make_token(PRINT),
        make_string_token("string"),
        make_token(ENDPOINT),
    };
    _IP = print10;
    printf("EXPECTED: Error...\nACTUAL: ");
    fflush(stdout);
    result = print_value(&table);
    assert(result == EXIT_FAILURE);
    printf("\n");

    // Unterminated: expression
    Token print11[] = {
        make_token(PRINT),
        make_identifier_token("_ident7"),
        make_token(PLUS),
        make_number_token(1.1),
        make_token(ENDPOINT),
    };
    _IP = print11;
    printf("EXPECTED: Error...\nACTUAL: ");
    fflush(stdout);
    result = print_value(&table);
    assert(result == EXIT_FAILURE);
    printf("\n");

    // Unterminated: nothing
    Token print12[] = {
        make_token(PRINT),
        make_token(ENDPOINT),
    };
    _IP = print12;
    printf("EXPECTED: Error...\nACTUAL: ");
    fflush(stdout);
    result = print_value(&table);
    assert(result == EXIT_FAILURE);
    printf("\n");


    destroy_variables(&table);
    return EXIT_SUCCESS;
}
