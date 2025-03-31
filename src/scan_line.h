#ifndef SCAN_LINE_H
#define SCAN_LINE_H

#include "lib/tokenlib.h"

// scan_line() can return a number of errors, some of which must be handled differently
typedef enum {
    SCAN_LINE_SUCCESS = 0,
    SCAN_LINE_FAILURE = 1, // Do not execute, but continue scanning
    SCAN_LINE_ABORT   = 2, // Stop scanning immediately
} ScanLineResult;


int scan_line(char *line, int line_num, TokenList *list);
char *get_substring(char *l, char *r);

#endif
