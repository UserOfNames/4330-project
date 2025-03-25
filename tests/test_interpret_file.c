#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test_interpret_file.h"
#include "../src/interpret_file.h"

int test_interpret_file() {
    FILE *file;

    // I will just assume this file doesn't exist
    assert(interpret_file("/this/path/should/not/exist\0\t\n\t\r\0") == EXIT_FAILURE);

    char *fname = "thisfileshouldnotexist\t\n\t\r\t\n";
    file = fopen(fname, "w");
    assert(interpret_file(fname) == EXIT_SUCCESS);
    remove(fname);
    fclose(file);

    return EXIT_SUCCESS;
}

