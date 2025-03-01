#include <stdio.h>
#include <stdlib.h>

#include "interpret.h"

#define EXIT_INVALID_PATH 5

int interpret(char* path_str) {
    FILE *path = fopen(path_str, "r");
    if (path == NULL) {
        fprintf(stderr, "Error: Could not open path %s\n", path_str);
        exit(EXIT_INVALID_PATH);
    }

    fclose(path);
    return EXIT_SUCCESS;
}
