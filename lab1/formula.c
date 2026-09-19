#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack.h"

/*
  [!] -> 0
  [+] -> 1
  [*] -> 2
  [^] -> 3
  [=] -> 4
  [->] -> 5
  [!->] -> 6
  [!+] -> 7
  [!*] -> 8
*/

short get_opcode(char *op) {
  if (!strcmp(op, "!")) {
    return 0;
  }
  if (!strcmp(op, "+")) {
    return 1;
  }
  if (!strcmp(op, "*")) {
    return 2;
  }
  if (!strcmp(op, "^")) {
    return 3;
  }
  if (!strcmp(op, "=")) {
    return 4;
  }
  if (!strcmp(op, "->")) {
    return 5;
  }
  if (!strcmp(op, "!->")) {
    return 6;
  }
  if (!strcmp(op, "!+")) {
    return 7;
  }
  if (!strcmp(op, "!*")) {
    return 8;
  }
  return -1;
}

char perform_operation(int opcode, int a, int b) {
  switch (opcode) {
    case 0:
      return (!a);
    case 1:
      return (a || b);
    case 2:
      return (a && b);
    case 3:
      return (a ^ b);
    case 4:
      return !(a ^ b);
    case 5:
      return (a <= b);
    case 6:
      return (a > b);
    case 7:
      return !(a || b);
    case 8:
      return !(a && b);
  }
}

int symbol_to_value(char symbol, char values[]) {
  if (symbol == '0' || symbol == '1') return (symbol - '0');
  symbol = toupper(symbol);
  return (values[symbol - 'A'] - '0');
}

int is_operator(char op) {
  return (op < 9 && op >= 0);
}

// Priority of every operation
short check_priority(int op) {
  switch (op) {
    case 0: return 6;
    case 1: return 3;
    case 2: return 5;
    case 3: return 4;
    case 4: return 1;
    case 5: return 2;
    case 6: return 2;
    case 7: return 3;
    case 8: return 5;
    default: return 0;
  }
}

stack_t* formula_to_postfix(FILE *file) {
  stack_t *stack = NULL;
  stack_t *result = NULL;
  while (!feof(file)) {
    int type = fgetc(file);
    int tmp;
    switch (type) {
      case '$':
        // Variable or constant value
        tmp = fgetc(file);
        stack_push(&result, tmp);
        break;
      case '[': {
        // Operator
        int i;
        char token_op[4];
        tmp = fgetc(file);
        for (i = 0; !feof(file) && tmp != ']' && (i < 3); ++i) {
          token_op[i] = tmp;
          tmp = fgetc(file);
        }
        token_op[i] = '\0';
        // Check for ! to include !!a case
        while (!stack_is_empty(stack)
              && (stack_top(stack) != 0
              ? (check_priority(stack_top(stack)) >= check_priority(get_opcode(token_op)))
              : (check_priority(stack_top(stack)) > check_priority(get_opcode(token_op))))) {
          stack_push(&result, stack_pop(&stack));
        }
        stack_push(&stack, get_opcode(token_op));
        break;
      }
      case '(':
        stack_push(&stack, '(');
        break;
      case ')':
        while (!stack_is_empty(stack) && stack_top(stack) != '(') {
          stack_push(&result, stack_pop(&stack));
        }
        if (!stack_is_empty(stack) && stack_top(stack) == '(') {
          stack_pop(&stack);
        }
        break;
      default:
        break;
    }
  }

  while (!stack_is_empty(stack)) {
    stack_push(&result, stack_pop(&stack));
  }

  // Reversing result stack
  stack_t *formula = NULL;
  while (!stack_is_empty(result)) {
    stack_push(&formula, stack_pop(&result));
  }
  return formula;
}

int compute_formula(stack_t *formula, int values, char symbols[]) {
  // symbols_with_value[i] = 1 if 'A'+i is in formula and its value is 1
  char symbols_with_values[27];
  strcpy(symbols_with_values, symbols);
  for (int i = 25; i >= 0; --i) {
    if (symbols_with_values[i] - '0') {
      symbols_with_values[i] = '0' + (values % 2);
      values /= 2;
    }
  }
  stack_t *expression = NULL;
  stack_t *formula_ptr = formula;
  while (formula_ptr != NULL) {
    if (!is_operator(stack_top(formula_ptr))) {
      stack_push(&expression, formula_ptr->value);
    } else {
      char op1 = stack_pop(&expression);
      char op2 = 0;
      if (formula_ptr->value != 0) {
        op2 = stack_pop(&expression);
        int op1_value = symbol_to_value(op1, symbols_with_values);
        int op2_value = symbol_to_value(op2, symbols_with_values);
        stack_push(&expression, perform_operation(formula_ptr->value, op2_value, op1_value)+'0');
      } else {
        // Operation is !
        int op1_value = symbol_to_value(op1, symbols_with_values);
        stack_push(&expression, perform_operation(formula_ptr->value, op1_value, 0)+'0');
      }
    }
    formula_ptr = formula_ptr->next;
  }
  if (stack_is_empty(expression)) return -1;
  // This is needed because of case when no operations are in formula
  return symbol_to_value(stack_top(expression), symbols_with_values) + '0';
}
