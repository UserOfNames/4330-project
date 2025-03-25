#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test_scan_line.h"
#include "../src/scan_line.h"

int test_scan_line() {
    // Case 1: Empty line
    TokenList list1;
    int result1 = scan_line("", 1, &list1);
    assert(result1 == EXIT_SUCCESS);

    // Case 2: Invalid line
    TokenList list2;
    int result2 = scan_line("ABCDE F G1 235", 1, &list2);
    assert(result2 == EXIT_FAILURE);

    return EXIT_SUCCESS;
}
