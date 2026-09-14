#include <stdlib.h>
#include <stdio.h>

struct stack_node {
  char *value;
  struct stack_node *next;
};

typedef struct stack_node stack_t;

void stack_push(stack_t **stack, char *value) {
  stack_t *ptr;
  ptr = malloc(sizeof(stack_t));
  if (ptr != NULL) {
    ptr->value = value;
    ptr->next = *stack;
    *stack = ptr;
  }
}

char* stack_pop(stack_t **stack) {
  stack_t *temp;
  char *pop_value;
  temp = *stack;
  pop_value = (*stack)->value;
  *stack = (*stack)->next;
  free(temp);
  return pop_value;
}

int stack_is_empty(stack_t **stack) {
  return ((*stack) == NULL);
}

char* stack_top(stack_t **stack) {
  if (stack_is_empty(stack)) return 0;
  return (*stack)->value;
}
