#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// Struct containing results from CLI arg parsing
#define LENGTHOF_ARG_RES 1 // Number of members in ArgResults
struct ArgResults {
    _Bool help;
};


void parse_args(int argc, char *argv[], struct ArgResults *arg_res, char **main_path) {
    int i;

    for (i=1; i<argc; i++) {
        // TODO make it more robust
        if (strncmp("-h", argv[i], 2) == 0) {
            arg_res -> help = true;
            return; // Help flag exits early, so no need to continue
        }

        // If it is none of the builtin flags, treat the arg as the path
        else {
            *main_path = argv[i];
        }
    }
}



void help() {
    printf("TODO HELP");

    exit(0);
}



int main(int argc, char *argv[]) {
    char *main_path = NULL;

    struct ArgResults arg_res;
    arg_res.help = false;

    parse_args(argc, argv, &arg_res, &main_path);

    if (arg_res.help)
        help();

    if (main_path == NULL) {
        printf("Error parsing path\n");
        exit(1);
    }

    printf("%s\n", main_path);

    // Parse args
        // File to interpret
        // Options
            // -h: help info, program info

    // Scan file
        // Construct AST

    // Walk AST

    return 0;
}
