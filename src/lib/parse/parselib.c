#include <stdio.h>
#include <stdlib.h>

#include "../tokenlib.h"
#include "assignment.h"
#include "variables.h"
#include "print.h"
#include "ifelse.h"
#include "while.h"

// Instruction pointer; pointer to current token being parsed
// Since this is accessed and mutated everywhere, it's useful to make it global
Token *_IP;

// Line number, tracked for error reporting; updated on each token
int line;


// Specific states, such as 'while,' 'ifelse,' 'expression,' 'assignment,' etc.
// are handled by their respective functions. When not in one of those states,
// we need a way to validate that the next token makes sense (i.e. that it will
// transition into one of those other states). This functions returns true if
// the next token makes sense for a base state and false if it doesn't.
// For simplicity, all baseline tokens must be associated with some directive:
// print, while, if, assignment, etc. Standalone expressions, literals,
// parentheses, blocks, etc., are disallowed.
int initial_state(Variable **table) {
    switch (_IP -> type) {
        // Note that IF and WHILE can recurse here
        case IF:
            return parse_ifelse(table);
            break;

        case WHILE:
            return parse_while(table);
            break;

        case PRINT:
            return print_value(table);
            break;

        // Semicolons should be ignored
        case SEMICOLON:
            _IP++;
            break;

        // Initial identifiers should correspond to assignment
        case IDENTIFIER:
            return parse_assignment(table);
            break;

        // Important to help IF/ELSE blocks find where they end
        case RCURLY:
            // For once, enums being integers actually helps!
            // (I hate C's type system)
            return RCURLY;

        // Similarly, we need to help WHILE blocks find their end
        case RSQUARE:
            return RSQUARE;

        // The end of the file should not be an error, so return it
        // for identification
        case ENDPOINT:
            return ENDPOINT;

        // All other tokens are invalid
        default:
            fprintf(stderr, "Error: Unexpected token %s on line %d\n",
                    get_token_string(*_IP), _IP -> line);
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
