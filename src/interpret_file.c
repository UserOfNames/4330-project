#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "interpret_file.h"
#include "interpret.h"

#define EXIT_INVALID_PATH 5

// If someone makes a line more than 256 characters long in a line-delimited
// language, they deserve to have their program crash
#define MAX_LINE_LENGTH 256


int interpret_file(char *path_str) {
    char line[MAX_LINE_LENGTH];

    FILE *path = fopen(path_str, "r");
    if (path == NULL) {
        fprintf(stderr, "Error: Could not open path %s\n", path_str);
        exit(EXIT_INVALID_PATH);
    }

    while (fgets(line, MAX_LINE_LENGTH, path) != NULL) {
        interpret(line);
    }

    fclose(path);
    return EXIT_SUCCESS;
}
