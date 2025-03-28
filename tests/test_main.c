#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test_interpret_file.h"
#include "test_scan_line.h"
#include "test_tokenlib.h"

int main(int argc, char *argv[]) {
    printf("\n---------------------------------\n"
             "| Output from tested functions |"
           "\n---------------------------------\n");


    printf("\n---------------------\n"
             "| interpret_file(): |"
           "\n---------------------\n");
    test_interpret_file();


    printf("\n-----------------\n"
             "| make_token(): |"
           "\n-----------------\n");
    test_make_token();


    printf("\n----------------------\n"
             "| make_token_list(): |"
           "\n----------------------\n");
    test_make_token_list();


    printf("\n----------------\n"
             "| add_token(): |"
           "\n----------------\n");
    test_add_token();


    printf("\n-----------------------\n"
             "| reset_token_list(): |"
           "\n-----------------------\n");
    test_reset_token_list();


    printf("\n----------------\n"
             "| scan_line(): |"
           "\n----------------\n");
    test_scan_line();


    printf("\n---------------------\n"
             "| All tests passing |"
           "\n---------------------\n");

    return EXIT_SUCCESS;
}
