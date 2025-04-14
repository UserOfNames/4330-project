#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenlib.h"

// Generate a new token
Token make_token(TokenType type) {
    Token token = {
        .type = type,
        .literal.Number = 0,
        .line = 0,
    };

    return token;
}


// Generate a new token with a literal value
Token make_token_with_literal(TokenType type, Literal literal) {
    Token token = make_token(type);
    token.literal = literal;

    return token;
}


// Print out a token list for debugging purposes
void print_token_list(TokenList *list) {
    int i;
    for (i=0; i<list->used; i++) {
        switch ((list -> tokens)[i].type) {
            case DISCARD:
                printf("DISCARD\n");
                break;
            case LPAREN:
                printf("LPAREN\n");
                break;
            case RPAREN:
                printf("RPAREN\n");
                break;
            case LCURLY:
                printf("LCURLY\n");
                break;
            case RCURLY:
                printf("RCURLY\n");
                break;
            case SEMICOLON:
                printf("SEMICOLON\n");
                break;
            case PLUS:
                printf("PLUS\n");
                break;
            case MINUS:
                printf("MINUS\n");
                break;
            case NEGATE:
                printf("NEGATE\n");
                break;
            case STAR:
                printf("STAR\n");
                break;
            case SLASH:
                printf("SLASH\n");
                break;
            case BANG:
                printf("BANG\n");
                break;
            case EQ:
                printf("EQ\n");
                break;
            case NOT_EQ:
                printf("NOT_EQ\n");
                break;
            case EQ_EQ:
                printf("EQ_EQ\n");
                break;
            case LT:
                printf("LT\n");
                break;
            case GT:
                printf("GT\n");
                break;
            case LT_EQ:
                printf("LT_EQ\n");
                break;
            case GT_EQ:
                printf("GT_EQ\n");
                break;
            case STRING:
                printf("STRING\n");
                break;
            case NUMBER:
                printf("NUMBER\n");
                break;
            case IDENTIFIER:
                printf("IDENTIFIER\n");
                break;
            case IF:
                printf("IF\n");
                break;
            case ELSE:
                printf("ELSE\n");
                break;
            case WHILE:
                printf("WHILE\n");
                break;
            case FALSE:
                printf("FALSE\n");
                break;
            case TRUE:
                printf("TRUE\n");
                break;
            case AND:
                printf("AND\n");
                break;
            case OR:
                printf("OR\n");
                break;
            case PRINT:
                printf("PRINT\n");
                break;
            case NONE:
                printf("NONE\n");
                break;
            case ENDPOINT:
                printf("ENDPOINT\n");
                break;
        }
    }
    printf("\n");
}


// Generate a numeric token
Token make_number_token(double num) {
    return make_token_with_literal(NUMBER, (Literal){.Number=num});
}

// Generate a string token
Token make_string_token(char *str) {
    return make_token_with_literal(STRING, (Literal){.String=strdup(str)});
}

// Generate an identifier token
Token make_identifier_token(char *name) {
    return make_token_with_literal(IDENTIFIER, (Literal){.Name=strdup(name)});
}

// Safely destroy a token, freeing associated memory if applicable
void destroy_token(Token *token) {
    switch(token -> type) {
        case STRING:
            free(token -> literal.String);
            token -> literal.Number = 0;
            token -> type = DISCARD;
            break;

        case NUMBER:
            token -> literal.Number = 0;
            token -> type = DISCARD;
            break;

        default:
            token -> type = DISCARD;
            break;
    }
}


// Create and return new token list
// Since a new list should always start with the same initial values,
// a constructor functions makes sense here
TokenList make_token_list() {
    TokenList list = {
        .tokens = NULL,
        .capacity = 0,
        .used = 0,
    };

    return list;
}


void reset_token_list(TokenList *list) {
    long i;
    Token *tokens = list -> tokens;

    if (tokens != NULL) {
        // Walk the token list and destroy each token
        for (i=0; i<list->used; i++) {
            destroy_token((list -> tokens) + i);
        }

        free(tokens);
    }

    list -> tokens = NULL;
    list -> capacity = list -> used = 0;
}


// Add a new token to the list
int add_token(TokenList *list, Token token) {
    long capacity = list -> capacity;
    long used     = list -> used;
    Token *temp;

    // If the list is full...
    if (capacity <= used) {
        // If the list is empty, set the capacity to default (16)
        // Otherwise, double the capacity
        list -> capacity = (capacity == 0) ? 16 : capacity * 2;

        // If realloc() fails, the original block is not freed
        // To prevent memory leaks, we must first verify its success
        // before reassigning the original pointer
        temp = (Token*)realloc(list -> tokens, list -> capacity * sizeof(Token));

        if (temp == NULL) {
            return EXIT_FAILURE;
        }

        list -> tokens = temp;
    }

    list -> tokens[list -> used] = token;
    list -> used++;

    return EXIT_SUCCESS;
}



// Add two numeric tokens
Token plus_tokens(Token l, Token r) {
    return make_token_with_literal (
        NUMBER,
        (Literal){.Number=l.literal.Number + r.literal.Number}
    );
}

// Subtract two numeric tokens
Token minus_tokens(Token l, Token r) {
    return make_token_with_literal (
        NUMBER,
        (Literal){.Number=l.literal.Number - r.literal.Number}
    );
}

// Negate a numeric token
Token negate_token(Token t) {
    return make_token_with_literal (
        NUMBER,
        (Literal){.Number=-t.literal.Number}
    );
}

// Multiply two numeric tokens
Token star_tokens(Token l, Token r) {
    return make_token_with_literal (
        NUMBER,
        (Literal){.Number=l.literal.Number * r.literal.Number}
    );
}

// Divide two numeric tokens
Token slash_tokens(Token l, Token r) {
    return make_token_with_literal (
        NUMBER,
        (Literal){.Number=l.literal.Number / r.literal.Number}
    );
}

// Negate a boolean token
Token bang_token(Token t) {
    if (t.type == TRUE)
        return make_token(FALSE);
    return make_token(TRUE);
}

// Compare two numeric tokens (!=)
Token not_eq_tokens(Token l, Token r) {
    if (l.literal.Number != r.literal.Number)
        return make_token(TRUE);
    return make_token(FALSE);
}

// Compare two numeric tokens (==)
Token eq_eq_tokens(Token l, Token r) {
    if (l.literal.Number == r.literal.Number)
        return make_token(TRUE);
    return make_token(FALSE);
}

// Compare two numeric tokens (<)
Token lt_tokens(Token l, Token r) {
    if (l.literal.Number < r.literal.Number)
        return make_token(TRUE);
    return make_token(FALSE);
}

// Compare two numeric tokens (>)
Token gt_tokens(Token l, Token r) {
    if (l.literal.Number > r.literal.Number)
        return make_token(TRUE);
    return make_token(FALSE);
}

// Compare two numeric tokens (<=)
Token lt_eq_tokens(Token l, Token r) {
    if (l.literal.Number <= r.literal.Number)
        return make_token(TRUE);
    return make_token(FALSE);
}

// Compare two numeric tokens (>=)
Token gt_eq_tokens(Token l, Token r) {
    if (l.literal.Number >= r.literal.Number)
        return make_token(TRUE);
    return make_token(FALSE);
}

// AND two boolean tokens
Token and_tokens(Token l, Token r) {
    if (l.type == TRUE && r.type == TRUE)
        return make_token(TRUE);
    return make_token(FALSE);
}

// OR two boolean tokens
Token or_tokens(Token l, Token r) {
    if (l.type == TRUE || r.type == TRUE)
        return make_token(TRUE);
    return make_token(FALSE);
}
