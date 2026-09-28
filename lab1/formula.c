#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack.c"

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

int symbol_to_value(char *symbol, char values[]) {
  if (symbol[0] == '0' || symbol[0] == '1') return (symbol[0] - '0');
  symbol[0] = toupper(symbol[0]);
  return (values[symbol[0] - 'A'] - '0');
}

int is_operator(char *op) {
  return (get_opcode(op) < 9 && get_opcode(op) >= 0);
}

// Priority of every operation
short check_priority(int opcode) {
  switch (opcode) {
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
  char *to_free = NULL;
  while (!feof(file)) {
    int type = fgetc(file);
    int tmp;
    switch (type) {
      case '$':
        // Variable or constant value
        tmp = fgetc(file);
        char token_operand[2];
        token_operand[0] = tmp;
        token_operand[1] = '\0';
        stack_push(&result, token_operand);
        break;
      case '[': {
        // Operator
        int i;
        char token_operator[4];
        tmp = fgetc(file);
        for (i = 0; !feof(file) && tmp != ']' && (i < 3); ++i) {
          token_operator[i] = tmp;
          tmp = fgetc(file);
        }
        token_operator[i] = '\0';
        // Check for ! to include !!a case
        while (!stack_is_empty(stack)
              && (stack_top(stack) != 0
              ? (check_priority(get_opcode(stack_top(stack))) >= check_priority(get_opcode(token_operator)))
              : (check_priority(get_opcode(stack_top(stack))) > check_priority(get_opcode(token_operator))))) {
          to_free = stack_pop(&stack);
          stack_push(&result, to_free);
          free(to_free);
        }
        stack_push(&stack, token_operator);
        break;
      }
      case '(': {
        char *brace_token = "(";
        stack_push(&stack, brace_token);
        break;
      }
      case ')':
        while (!stack_is_empty(stack) && strcmp(stack_top(stack), "(")) {
          to_free = stack_pop(&stack);
          stack_push(&result, to_free);
          free(to_free);
        }
        if (!stack_is_empty(stack) && !strcmp(stack_top(stack), "(")) {
          to_free = stack_pop(&stack);
          free(to_free);
        }
        break;
      default:
        break;
    }
  }

  rewind(file);

  while (!stack_is_empty(stack)) {
    to_free = stack_pop(&stack);
    stack_push(&result, to_free);
    free(to_free);
  }

  // Reversing result stack
  stack_t *formula = NULL;
  while (!stack_is_empty(result)) {
    to_free = stack_pop(&result);
    stack_push(&formula, to_free);
    free(to_free);
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
      char* op1 = stack_pop(&expression);
      char* op2 = "";
      if (get_opcode(formula_ptr->value) != 0) {
        op2 = stack_pop(&expression);
        int op1_value = symbol_to_value(op1, symbols_with_values);
        int op2_value = symbol_to_value(op2, symbols_with_values);
        char calc_result[2];
        calc_result[0] = perform_operation(get_opcode(formula_ptr->value), op2_value, op1_value) + '0';
        calc_result[1] = '\0';
        stack_push(&expression, calc_result);
        free(op2);
      } else {
        // Operation is !
        int op1_value = symbol_to_value(op1, symbols_with_values);
        char calc_result[2];
        calc_result[0] = perform_operation(get_opcode(formula_ptr->value), op1_value, 0) + '0';
        calc_result[1] = '\0';
        stack_push(&expression, calc_result);
      }
      free(op1);
    }
    formula_ptr = formula_ptr->next;
  }
  if (stack_is_empty(expression)) return -1;
  // This is needed because of case when no operations are in formula
  char *expr_top = stack_pop(&expression);
  char return_value = symbol_to_value(expr_top, symbols_with_values) + '0';
  free(expr_top);
  return return_value;
}
