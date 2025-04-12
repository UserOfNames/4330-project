#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../tokenlib.h"
#include "parselib.h"
#include "expressions.h"
#include "queue.h"
#include "token_stack.h"
#include "variables.h"

// Get the precedence of an operator; higher number is higher precedence
int get_precedence(TokenType operator) {
    switch (operator) {
        case PLUS:
            return 4;
        case MINUS:
            return 4;
        case NEGATE:
            return 7;
        case STAR:
            return 5;
        case SLASH:
            return 5;
        case AND:
            return 1;
        case OR:
            return 0;
        case BANG:
            return 6;
        case NOT_EQ:
            return 2;
        case EQ_EQ:
            return 2;
        case LT:
            return 3;
        case GT:
            return 3;
        case LT_EQ:
            return 3;
        case GT_EQ:
            return 3;
        default:
            return -1;
    }
}

// Execute repetitive error logic
void pe_err(Token *res, _Bool *break_loop) {
    fprintf(stderr, "Error evaluating expression on line %d\n", line);
    res -> type = DISCARD;
    *break_loop = true;
}

// Indicate invalid types passed to an operand
void pe_operand_err(Token *res, _Bool *break_loop, char *op) {
    fprintf(stderr, "Error: Invalid types for operator \n");
    res -> type = DISCARD;
    *break_loop = true;
}

// Handle an operator for parse_expression()
void pe_handle_operator(Token operator, Queue *output, TokenStack *operators) {
    int precedence = get_precedence(operator.type);

    while (
        !is_empty_token(operators) &&
        top_token(operators).type != LPAREN &&
        get_precedence(top_token(operators).type) >= precedence
    ) enqueue(output, pop_token(operators));

    push_token(operators, operator);
}

// Check whether two operands are both boolean
_Bool are_boolean(Token l, Token r) {
    return ((l.type == TRUE || l.type == FALSE) &&
            (r.type == TRUE || r.type == FALSE));
}

Token evaluate_queue(Queue *q) {
    TokenStack s = make_token_stack();
    Token result;
    Token c, l, r;

    _Bool break_loop = false;
    while (q -> length > 0 && !break_loop) {
        c = dequeue(q);
        switch (c.type) {
            case NUMBER:
                push_token(&s, c);
                break;

            case TRUE:
                push_token(&s, c);
                break;

            case FALSE:
                push_token(&s, c);
                break;

            case PLUS:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                // First popped -> second operand
                r = pop_token(&s);
                l = pop_token(&s);
                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, plus_tokens(l, r));
                break;

            case MINUS:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);
                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, minus_tokens(l, r));
                break;

            case NEGATE:
                if (s.used < 1) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                if (r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, negate_token(r));
                break;

            case STAR:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);
                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, star_tokens(l, r));
                break;

            case SLASH:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);
                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, slash_tokens(l, r));
                break;

            case BANG:
                if (s.used < 1) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);

                if (r.type != TRUE && r.type != FALSE) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, bang_token(r));
                break;

            case NOT_EQ:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);

                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, not_eq_tokens(l, r));
                break;

            case EQ_EQ:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);

                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, eq_eq_tokens(l, r));
                break;

            case LT:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);
                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, lt_tokens(l, r));
                break;

            case GT:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);
                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, gt_tokens(l, r));
                break;

            case LT_EQ:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);
                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, lt_eq_tokens(l, r));
                break;

            case GT_EQ:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);
                if (l.type != NUMBER || r.type != NUMBER) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, gt_tokens(l, r));
                break;

            case AND:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);
                if (!are_boolean(r, l)) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, and_tokens(l, r));
                break;

            case OR:
                if (s.used < 2) {
                    pe_err(&result, &break_loop);
                    break;
                }

                r = pop_token(&s);
                l = pop_token(&s);
                if (!are_boolean(r, l)) {
                    pe_err(&result, &break_loop);
                    break;
                }

                push_token(&s, or_tokens(l, r));
                break;

            default:
                pe_err(&result, &break_loop);
                break;
        }
    }

    // There should be exactly one value in the stack now, otherwise the
    // expression is invalid
    if (s.used != 1) {
        fprintf(stderr, "Error evaluating expression on line %d\n"
                        "evaluate_queue: s.used != 1 after evaluation\n", c.line);
        result.type = DISCARD;
    } else {
        result = top_token(&s);
    }

    destroy_token_stack(&s);
    return result;
}


// When IP finds a number, that indicates an arithmetic expression.
// When this is called, IP should be pointing to that first number.
// This evaluates the expression using a modified shunting yard algorithm
Token parse_expression(Variable **table) {
    Queue output = make_queue();
    TokenStack operators = make_token_stack();

    // A variable retrieved from the variable map
    Token *var;

    // This type may be changed, but it needs to be initialized
    // to something besides DISCARD since that's used for errors
    Token result = {.type=NUMBER};

    // Number of consecutive negations
    int neg_count = 0;

    _Bool break_loop = false;
    while (_IP -> type != ENDPOINT && _IP -> type != SEMICOLON && !break_loop) {
        line = _IP -> line;
        switch (_IP -> type) {
            case NUMBER:
                enqueue(&output, *_IP);
                _IP++;
                break;

            case LPAREN:
                push_token(&operators, *_IP);
                _IP++;
                break;

            case RPAREN:
                while (top_token(&operators).type != LPAREN) {
                    enqueue(&output, pop_token(&operators));
                    if (operators.used <= 0) {
                        fprintf(stderr, "Error: Mismatched parentheses in expression on line %d\n", line);
                        result.type = DISCARD;
                        break_loop = true;
                        break;
                    }
                }

                if (!break_loop) {
                    pop_token(&operators);
                };

                _IP++;
                break;

            case IDENTIFIER:
                var = get_variable(table, _IP -> literal.Name);
                if (var == NULL) {
                    fprintf(stderr, "Error: Variable '%s' on line %d not found\n", _IP -> literal.Name, line);
                    result.type = DISCARD;
                    break_loop = true;
                    break;
                }

                switch (var -> type) {
                    case NUMBER:
                        enqueue(&output, *var);
                        break;
                    case TRUE:
                        enqueue(&output, *var);
                        break;
                    case FALSE:
                        enqueue(&output, *var);
                        break;
                    default:
                        fprintf(stderr, "Error: Variable '%s' on line %d is of an invalid type\n", _IP -> literal.Name, line);
                        result.type = DISCARD;
                        break_loop = true;
                        break;
                }
                _IP++;
                break;

            case TRUE:
                enqueue(&output, *_IP);
                _IP++;
                break;

            case FALSE:
                enqueue(&output, *_IP);
                _IP++;
                break;

            case PLUS:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case MINUS:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case NEGATE:
                // Consume all consecutive negations; even = no effect, odd = negate
                neg_count = 0;
                while (_IP -> type == NEGATE && _IP -> type != ENDPOINT) {
                    neg_count++;
                    _IP++;
                }

                // Parity check
                if ((neg_count & 1) == 0)
                    break;
                pe_handle_operator(_IP[-1], &output, &operators);
                break;

            case STAR:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case SLASH:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case BANG:
                // Consume all NOTs; even = no effect, odd = negate
                neg_count = 0;
                while (_IP -> type == BANG && _IP -> type != ENDPOINT) {
                    neg_count++;
                    _IP++;
                }

                if ((neg_count & 1) == 0)
                    break;
                pe_handle_operator(_IP[-1], &output, &operators);
                break;

            case EQ_EQ:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case NOT_EQ:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case LT:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case GT:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case LT_EQ:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case GT_EQ:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case AND:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            case OR:
                pe_handle_operator(*_IP, &output, &operators);
                _IP++;
                break;

            // All other types are invalid for an arithmetic expression,
            // so throw an error
            default:
                fprintf(stderr, "Error: Unexpected token in expression on line %d\n", line);
                result.type = DISCARD;
                break_loop = true;
                break;
        }
    }
    // End of initial while loop

    // If there was an error, skip evaluation
    if (result.type == DISCARD) {
        destroy_queue(&output);
        destroy_token_stack(&operators);
        return result;
    }

    while (operators.used > 0) {
        if (top_token(&operators).type == LPAREN) {
            fprintf(stderr, "Error: Mismatched parentheses in expression on line %d\n", line);
            result.type = DISCARD;
            destroy_queue(&output);
            destroy_token_stack(&operators);
            return result;
        }

        enqueue(&output, pop_token(&operators));
    }

    result = evaluate_queue(&output);

    destroy_queue(&output);
    destroy_token_stack(&operators);
    return result;
}
