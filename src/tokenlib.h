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

    DOT,

    PLUS, MINUS, STAR, SLASH,

    HASH,

    BANG,

    EQ, NOT_EQ, EQ_EQ,
    LT, GT, LT_EQ, GT_EQ,

    STRING,
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


Token make_token(TokenType type);
Token make_token_lexeme(TokenType type, char *lexeme);
TokenList make_token_list();
void reset_token_list(TokenList *list);
int add_token(TokenList *list, Token token);

#endif
