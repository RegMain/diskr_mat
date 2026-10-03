#pragma once

struct stack_node {
  char *value;
  struct stack_node *next;
};

typedef struct stack_node stack_t;

void stack_push(stack_t **stack, char *value);

char* stack_pop(stack_t **stack);

int stack_is_empty(stack_t *stack);

char* stack_top(stack_t *stack);
