#include <stdio.h>
#include <stdlib.h>

#include "interpret.h"

#define EXIT_INVALID_PATH 5

#define MAX_LINE_LENGTH 128

int interpret(char* path_str) {
    char line[MAX_LINE_LENGTH];

    FILE *path = fopen(path_str, "r");
    if (path == NULL) {
        fprintf(stderr, "Error: Could not open path %s\n", path_str);
        exit(EXIT_INVALID_PATH);
    }

    while (fgets(line, MAX_LINE_LENGTH, path) != NULL) {
        ;
    }

    fclose(path);
    return EXIT_SUCCESS;
}
