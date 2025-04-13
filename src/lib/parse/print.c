#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../tokenlib.h"
#include "expressions.h"
#include "variables.h"
#include "parselib.h"

int print_value(Variable **table) {
    // IP is currently pointing to the 'print' directive;
    // increment to the next token, then figure out how to handle it
    _IP++;

    Token result, *var_result;

    switch (_IP -> type) {
        case STRING:
            printf("%s\n", _IP -> literal.String);
            break;

        case IDENTIFIER:
            // We must first check if the variable exists...
            var_result = get_variable(table, _IP -> literal.Name);
            if (var_result == NULL) {
                fprintf(stderr, "Error: Variable '%s' on line %d not found\n", _IP -> literal.Name, line);
                return EXIT_FAILURE;
            }

            // ...then if it's none...
            if (var_result -> type == NONE) {
                printf("None\n");
                return EXIT_SUCCESS;
            }

            // ...then if it's a string...
            if (var_result -> type == STRING) {
                printf("%s\n", var_result -> literal.String);
                return EXIT_SUCCESS;
            }

            // ...then we can assume it represents an expression
            // parse_expression() will catch the other possible errors
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }

            switch (result.type) {
                case NUMBER:
                    printf("%f\n", result.literal.Number);
                    break;

                case TRUE:
                    printf("True\n");
                    break;

                case FALSE:
                    printf("False\n");
                    break;

                case DISCARD:
                    return EXIT_FAILURE;

                default:
                    fprintf(stderr, "Unknown error parsing expression on line %d\n", _IP -> line);
                    return EXIT_FAILURE;
            }

            break;

        case NUMBER:
            // Check that the expression parse was successful first
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }

            switch (result.type) {
                case NUMBER:
                    printf("%f\n", result.literal.Number);
                    break;

                case TRUE:
                    printf("True\n");
                    break;

                case FALSE:
                    printf("False\n");
                    break;

                case DISCARD:
                    return EXIT_FAILURE;

                default:
                    fprintf(stderr, "Unknown error parsing expression on line %d\n", _IP -> line);
                    return EXIT_FAILURE;
            }

            break;

        case TRUE:
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }

            switch (result.type) {
                case TRUE:
                    printf("True\n");
                    break;

                case FALSE:
                    printf("False\n");
                    break;

                case DISCARD:
                    return EXIT_FAILURE;

                default:
                    fprintf(stderr, "Unknown error parsing expression on line %d\n", _IP -> line);
                    return EXIT_FAILURE;
            }

            break;

        case FALSE:
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }

            switch (result.type) {
                case TRUE:
                    printf("True\n");
                    break;

                case FALSE:
                    printf("False\n");
                    break;

                case DISCARD:
                    return EXIT_FAILURE;

                default:
                    fprintf(stderr, "Unknown error parsing expression on line %d\n", _IP -> line);
                    return EXIT_FAILURE;
            }

            break;

        case NONE:
            printf("None\n");
            break;

        default:
            fprintf(stderr, "Error: Invalid value passed to 'print' on line %d\n", 
                    _IP -> line);
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
