#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main() {
  stack_t *stack;
  for (char i = 'a'; i <= 'z'; ++i) {
    stack_push(&stack, i);
  }
  while (!stack_is_empty(&stack)) {
    printf("%c", stack_pop(&stack));
  }
  return 0;
}
 
