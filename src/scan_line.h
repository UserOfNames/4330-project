#ifndef INTERPRET_H
#define INTERPRET_H

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

TokenList make_token_list();

int add_token(TokenList *list, Token token);

int scan_line(char *line, int line_number, TokenList *list);

#endif
