#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#include "../tokenlib.h"

// Node for the linked list underlying the queue implementation
typedef struct Node Node;
struct Node {
    Token token;
    Node *next;
};

// A queue used to track numbers for the shunting yard algorithm
typedef struct {
    Node *head;
    Node *tail;
    long length;
} Queue;

Queue make_queue();
Node* make_node(Token token);
int enqueue(Queue *queue, Token token);
Token dequeue(Queue *queue);
Token peek(Queue *queue);
void destroy_queue(Queue *queue);

#endif
