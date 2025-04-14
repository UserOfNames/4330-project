#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> // For getopt

#include "interpret_file.h"

// Struct containing results from CLI arg parsing
struct ArgResults {
    _Bool help;
    _Bool quiet;
};


void parse_args(int argc, char *argv[], struct ArgResults *arg_res) {
    int c;

    opterr = 0;

    // Using GNU getopt extension that permutes argv
    // Not POSIX compliant
    while ((c = getopt(argc, argv, "hq")) != -1) {
        switch (c) {
            case '?':
                fprintf(stderr, "Error: Invalid option '%c'\n", optopt);
                exit(EXIT_FAILURE);

            case 'h':
                arg_res -> help = true;
                break;

            case 'q':
                arg_res -> quiet = true;
                break;
        }
    }
}



void help(char *program_name) {
    printf("Interpreter for a very basic custom programming language.\n\n"
           "Usage: %s <options> <path to source file>\n\n"
           "Options:\n"
           "-h: Help: Show this help text.\n"
           "-q: Quiet: Suppress intermediate output.\n"
           , program_name);

    exit(EXIT_SUCCESS);
}



int main(int argc, char *argv[]) {
    struct ArgResults arg_res = {false};

    parse_args(argc, argv, &arg_res);

    // We run this first because the program exits if help is printed
    // We don't care about arg counts in this case, so those checks come next
    if (arg_res.help)
        help(argv[0]);


    if (optind == argc) {
        fprintf(stderr, "Error: No path given\n");
        exit(EXIT_FAILURE);
    }
    // Once all options are parsed, the only remaining argument should be the
    // file to interpret, i.e. at most 1 argument
    else if (argc - optind > 1) {
        fprintf(stderr, "Error: Must give only one path\nUse option -h for help\n");
        exit(EXIT_FAILURE);
    }

    
    int stdout_copy;
    if ((stdout_copy = dup(STDOUT_FILENO)) == -1) {
        perror("Error copying stdout:");
        exit(EXIT_FAILURE);
    }

    if (arg_res.quiet) {
        if ((freopen("/dev/null", "w", stdout)) == NULL) {
            perror("Error redirecting stdout:");
            exit(EXIT_FAILURE);
        }
    }


    int result = interpret_file(argv[optind]);

    if (result != 0)
        fprintf(stderr, "Could not interpret %s\n", argv[optind]);


    if (arg_res.quiet) {
        fflush(stdout);
        dup2(stdout_copy, STDOUT_FILENO);
        close(stdout_copy);
    }


    return result;
}
