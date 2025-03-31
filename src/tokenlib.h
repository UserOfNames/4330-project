#ifndef TOKENLIB_H
#define TOKENLIB_H

/*
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Collection of functions and data structures to represent the token stream *
 *                                                                           *
 *---------------------------------------------------------------------------*
*/

// Enum representing the type of a given token
typedef enum {
    // Default type of token
    // Indicates a token should not be added to the list
    DISCARD,

    LPAREN, RPAREN,

    LCURLY, RCURLY,

    DOT,

    PLUS, MINUS, STAR, SLASH,

    HASH,

    BANG,

    EQ, NOT_EQ, EQ_EQ,
    LT, GT, LT_EQ, GT_EQ,

    STRING, NUMBER, IDENTIFIER,

    // Keywords
    IF, ELSE, WHILE, FALSE, TRUE,
    AND, OR, LET, PRINT, NONE,
} TokenType;


// Active field should be determined by Token.type
typedef union {
    double Number;
    char *String;
} Literal;


// Struct representing a single token
typedef struct {
    TokenType type;
    Literal literal;
} Token;


// Dynamic array of tokens
typedef struct {
    Token *tokens;
    long capacity;
    long used;
} TokenList;


// Token generation
Token make_token(TokenType type);
Token make_token_with_literal(TokenType type, Literal literal);

// Actions on token list
TokenList make_token_list();
void reset_token_list(TokenList *list);
int add_token(TokenList *list, Token token);

#endif
