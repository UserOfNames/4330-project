#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

#include "../tokenlib.h"
#include "expressions.h"
#include "variables.h"
#include "parselib.h"

// Print an error signifying the lack of a semicolon after the print statement
void unterminated_err() {
    fprintf(stderr, "Error: Unterminated print statment on line %d\n"
            "Print statements should be terminated with a ';'\n",
            _IP -> line);
}

// Make the output of number printing look nice
void print_double(double num) {
    double int_part;
    double frac_part = modf(num, &int_part);

    // Check if the fractional part is close to zero. If so, print as an
    // integer. If not, print as a float
    if (fabs(frac_part) < DBL_EPSILON) {
        printf("%.0f\n", int_part);
    } else {
        printf("%f\n", num);
    }
}

int print_value(Variable **table) {
    // IP is currently pointing to the 'print' directive;
    // increment to the next token, then figure out how to handle it
    _IP++;

    Token result, *var_result;

    switch (_IP -> type) {
        case STRING:
            printf("%s\n", _IP -> literal.String);
            // Make sure the print statement is terminated
            _IP++;
            if (_IP -> type != SEMICOLON) {
                unterminated_err();
                return EXIT_FAILURE;
            }
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
                _IP++;
                if (_IP -> type != SEMICOLON) {
                    unterminated_err();
                    return EXIT_FAILURE;
                }

                return EXIT_SUCCESS;
            }

            // ...then if it's a string...
            if (var_result -> type == STRING) {
                printf("%s\n", var_result -> literal.String);
                _IP++;
                if (_IP -> type != SEMICOLON) {
                    unterminated_err();
                    return EXIT_FAILURE;
                }

                return EXIT_SUCCESS;
            }

            // ...then we can assume it represents an expression
            // parse_expression() will catch the other possible errors
            // Note that expression must already terminate with a semicolon,
            // so we don't need to check that again in this case
            result = parse_expression(table);

            switch (result.type) {
                case NUMBER:
                    print_double(result.literal.Number);
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
            result = parse_expression(table);

            switch (result.type) {
                case NUMBER:
                    print_double(result.literal.Number);
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

        case NEGATE:
            result = parse_expression(table);

            switch (result.type) {
                case NUMBER:
                    print_double(result.literal.Number);
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

        case BANG:
            result = parse_expression(table);

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
            _IP++;
            if (_IP -> type != SEMICOLON) {
                unterminated_err();
                return EXIT_FAILURE;
            }
            printf("None\n");
            break;

        default:
            fprintf(stderr, "Error: Invalid value passed to 'print' on line %d\n", 
                    _IP -> line);
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
