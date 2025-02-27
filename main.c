#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


// Struct containing results from CLI arg parsing
struct ArgResults {
    _Bool help;
};


void parse_args(int argc, char *argv[], struct ArgResults *arg_res) {
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "h")) != -1) {
        switch (c) {
            case '?':
                printf("Error: Invalid option '%c'\n", optopt);
                exit(1);

            case 'h':
                arg_res -> help = true;
                break;
        }
    }

    // Once all options are parsed, the only remaining argument should be the
    // file to interpret, i.e. at most 1 argument (may be 0, e.g. if -h is passed)
    if (argc - optind > 1) {
        printf("Error: Must give only one path\nUse option -h for help\n");
        exit(1);
    }
}



// FIXME: PLACEHOLDER NAME
void help(char *program_name) {
    printf("Interpreter for FIXME PLACEHOLDER, a very basic custom programming language.\n\n"
           "Usage: %s <options> <path to source file>\n\n"
           "Options:\n"
           "-h: Show this help text.\n"
           , program_name);

    exit(0);
}



int main(int argc, char *argv[]) {
    struct ArgResults arg_res = {false};
    arg_res.help = false;

    parse_args(argc, argv, &arg_res);

    if (arg_res.help)
        help(argv[0]);

    if (optind == argc) {
        printf("Error: No path given\n");
        exit(1);
    }

    // Scan file
        // Construct AST

    // Walk AST

    return 0;
}
