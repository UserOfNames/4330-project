#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test_interpret_file.h"
#include "test_scan_line.h"

int main(int argc, char *argv[]) {
    printf("\n---------------------------------\n"
           "| Output from tested functions: |"
           "\n---------------------------------\n\n");

    test_interpret_file();
    test_scan_line();

    printf("\n---------------------\n"
           "| All tests passing |"
           "\n---------------------\n");

    return EXIT_SUCCESS;
}
