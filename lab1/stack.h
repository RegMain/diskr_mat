#include <stdlib.h>
#include <stdio.h>

struct stack_node {
  int value;
  struct stack_node *next;
};

typedef struct stack_node stack_t;

void stack_push(stack_t **stack, int value) {
  stack_t *ptr;
  ptr = malloc(sizeof(stack_t));
  if (ptr != NULL) {
    ptr->value = value;
    ptr->next = *stack;
    *stack = ptr;
  }
}

int stack_pop(stack_t **stack) {
  if (!stack) return -1;
  stack_t *temp;
  int pop_value;
  temp = *stack;
  pop_value = (*stack)->value;
  *stack = (*stack)->next;
  free(temp);
  return pop_value;
}

int stack_is_empty(stack_t *stack) {
  return (stack == NULL);
}

int stack_top(stack_t *stack) {
  if (stack_is_empty(stack)) return -1;
  return (stack->value);
}
