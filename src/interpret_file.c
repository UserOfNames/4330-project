#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "interpret_file.h"
#include "lib/tokenlib.h"
#include "scan_line.h"
#include "constants.h"
#include "lib/parse/parselib.h"


int interpret_file(char *path_str) {
    // One extra byte for null-terminator, one for trailing newline
    char line[MAX_LINE_LENGTH + 2] = {0};
    int result = EXIT_SUCCESS;

    FILE *path = fopen(path_str, "r");
    if (path == NULL) {
        fprintf(stderr, "Error: Could not open path %s\n", path_str);
        return EXIT_FAILURE;
    }


    // Iterate over the file line-by-line, passing each line to the scanner
    int line_number = 0;
    TokenList token_list = make_token_list();

    _Bool break_loop = false;
    while (fgets(line, MAX_LINE_LENGTH + 2, path) != NULL && !break_loop) {
        line_number++;

        // Ensure the line does not exceed the limit
        // fgets() null terminates the buffer, so plain strlen() is safe here
        if (line[strlen(line) - 1] != '\n') {
            fprintf(stderr, "Error: Line %d exceeds %d characters.\n", line_number, MAX_LINE_LENGTH);
            result = EXIT_FAILURE;
            break;
        }

        switch (scan_line(line, line_number, &token_list)) {
            SCAN_LINE_SUCCESS:
                break;

            SCAN_LINE_FAILURE:
                result = EXIT_FAILURE;
                break;

            SCAN_LINE_ABORT:
                result = EXIT_FAILURE;
                break_loop = true;
                break;
        }
    }

    // If tokenization failed or produced no tokens, skip parsing
    if (token_list.used == 0 || result != EXIT_SUCCESS) {
        break_loop = true;
    } else {
        // Terminate the token list
        Token end = make_token(ENDPOINT);
        end.line = (token_list.tokens)[token_list.used-1].line;
        add_token(&token_list, end);
    }

    // Initialize the variable hashmap
    Variable *table = NULL;

    // Initialize the instruction pointer
    _IP = token_list.tokens;

    int parse_result;
    while (!break_loop && (parse_result = initial_state(&table, false, false)) != ENDPOINT) {
        // Some type system abuse here, using enum variants alongside exit
        // codes. C's awful type system not only allows this abuse,
        // it almost forces it if you don't want to make your life miserable.
        // So here we go!
        switch (parse_result) {
            case EXIT_FAILURE:
                result = EXIT_FAILURE;
                break_loop = true;
                break;

            // There shouldn't be a } in this state
            case RCURLY:
                fprintf(stderr, "Error: Unexpected '}' on line %d\n", _IP -> line);
                result = EXIT_FAILURE;
                break_loop = true;
                break;

            // Nor should there be a ]
            case RSQUARE:
                fprintf(stderr, "Error: Unexpected ']' on line %d\n", _IP -> line);
                result = EXIT_FAILURE;
                break_loop = true;
                break;
        }
    }

    // Cleanup
    destroy_variables_after_token_list(&table);
    reset_token_list(&token_list);
    fclose(path);
    path = NULL;

    return result;
}
