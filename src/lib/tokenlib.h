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

    SEMICOLON,

    PLUS, MINUS, NEGATE, STAR, SLASH,

    // Logical NOT
    BANG,

    EQ, NOT_EQ, EQ_EQ,
    LT, GT, LT_EQ, GT_EQ,

    // Literals
    STRING, NUMBER, IDENTIFIER,

    // Keywords
    IF, ELSE, WHILE, FALSE, TRUE,
    AND, OR, PRINT, NONE,

    // Special token denoting the end of a token list
    ENDPOINT,
} TokenType;


// Active field should be determined by Token.type
typedef union {
    double Number; // Number literal
    char *String;  // String literal
    char *Name;    // Identifier name
} Literal;


// Struct representing a single token
typedef struct {
    TokenType type;
    Literal literal;
    int line; // Line number associated with the token
} Token;


// Dynamic array of tokens
typedef struct {
    Token *tokens;
    long capacity;
    long used;
} TokenList;


// Token actions
Token make_token(TokenType type);
Token make_token_with_literal(TokenType type, Literal literal);
Token make_number_token(double num);
Token make_string_token(char *str);
Token make_identifier_token(char *name);
void destroy_token(Token *token);

// Actions on token list
TokenList make_token_list();
void reset_token_list(TokenList *list);
int add_token(TokenList *list, Token token);

Token plus_tokens(Token l, Token r);
Token minus_tokens(Token l, Token r);
Token negate_token(Token t);
Token star_tokens(Token l, Token r);
Token slash_tokens(Token l, Token r);
Token bang_token(Token t);
Token not_eq_tokens(Token l, Token r);
Token eq_eq_tokens(Token l, Token r);
Token lt_tokens(Token l, Token r);
Token gt_tokens(Token l, Token r);
Token lt_eq_tokens(Token l, Token r);
Token gt_eq_tokens(Token l, Token r);
Token and_tokens(Token l, Token r);
Token or_tokens(Token l, Token r);

#endif
