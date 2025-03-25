#ifndef INTERPRET_H
#define INTERPRET_H

// Maximum line length allowed by the interpreter
#define MAX_LINE_LENGTH 500


// Enum representing the type of a given token
typedef enum {
    // Default type of token
    // Indicate a token should not be added to the list
    DISCARD,

    LPAREN, RPAREN,

    PLUS, MINUS, STAR,

    DOUBLE_QUOTE,
} TokenType;

// Struct representing a single token
typedef struct {
    TokenType type;
    char *lexeme;
} Token;

// Dynamic array of tokens
typedef struct {
    Token *tokens;
    long capacity;
    long used;
} TokenList;


// scan_line() can return a number of errors, some of which must be handled differently
typedef enum {
    SCAN_LINE_SUCCESS = 0,
    SCAN_LINE_FAILURE = 1, // Do not execute, but continue scanning
    SCAN_LINE_ABORT   = 2, // Stop scanning immediately
} ScanLineResult;


TokenList make_token_list();

int scan_line(char *line, int line_number, TokenList *list);

#endif
