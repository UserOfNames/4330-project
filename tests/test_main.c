#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib/parse/test_assignment.h"
#include "test_interpret_file.h"
#include "lib/test_tokenlib.h"
#include "test_scan_line.h"
#include "lib/parse/test_stack.h"
#include "lib/parse/test_queue.h"
#include "lib/parse/test_variables.h"
#include "lib/parse/test_expressions.h"

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


    printf("\n------------------------------\n"
             "| make_token_with_literal(): |"
           "\n------------------------------\n");
    test_make_token_with_literal();


    printf("\n------------------------\n"
             "| make_number_token(): |"
           "\n------------------------\n");
    test_make_number_token();


    printf("\n------------------------\n"
             "| make_string_token(): |"
           "\n------------------------\n");
    test_make_string_token();


    printf("\n----------------------------\n"
             "| make_identifier_token(): |"
           "\n----------------------------\n");
    test_make_identifier_token();


    printf("\n--------------------\n"
             "| destroy_token(): |"
           "\n--------------------\n");
    test_destroy_token();


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


    printf("\n--------------------\n"
             "| get_substring(): |"
           "\n--------------------\n");
    test_get_substring();


    printf("\n----------------\n"
             "| scan_line(): |"
           "\n----------------\n");
    test_scan_line();


    printf("\n-----------------\n"
             "| make_stack(): |"
           "\n-----------------\n");
    test_make_ptr_stack();


    printf("\n-------------------\n"
             "|destroy_stack(): |"
           "\n-------------------\n");
    test_destroy_ptr_stack();


    printf("\n-----------\n"
             "| push(): |"
           "\n-----------\n");
    test_push_ptr();


    printf("\n----------\n"
             "| top(): |"
           "\n----------\n");
    test_top_ptr();


    printf("\n----------\n"
             "| pop(): |"
           "\n----------\n");
    test_pop_ptr();


    printf("\n-----------------\n"
             "| make_queue(): |"
           "\n-----------------\n");
    test_make_queue();


    printf("\n----------------\n"
             "| make_node(): |"
           "\n----------------\n");
    test_make_node();


    printf("\n--------------------\n"
             "| destroy_queue(): |"
           "\n--------------------\n");
    test_destroy_queue();


    printf("\n--------------\n"
             "| enqueue(): |"
           "\n--------------\n");
    test_enqueue();


    printf("\n--------------\n"
             "| dequeue(): |"
           "\n--------------\n");
    test_dequeue();


    printf("\n-----------\n"
             "| peek(): |"
           "\n-----------\n");
    test_peek();


    printf("\n--------------\n"
             "| variables: |"
           "\n--------------\n");
    test_variables();


    printf("\n-----------------------\n"
             "| parse_expression(): |"
           "\n-----------------------\n");
    test_parse_expression();


    printf("\n-----------------------\n"
             "| parse_assignment(): |"
           "\n-----------------------\n");
    test_parse_assignment();


    printf("\n---------------------\n"
             "| All tests passing |"
           "\n---------------------\n");

    return EXIT_SUCCESS;
}
