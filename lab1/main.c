#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int is_operator(char op) {
  return (op < 9 && op >= 0);
}

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
  // TODO
  while (!feof(file)) {
    int type = fgetc(file);
    int tmp;
    switch (type) {
      case '$':
        tmp = fgetc(file);
        stack_push(&result, tmp);
        break;
      case '[': {
        int i;
        char token_op[4];
        tmp = fgetc(file);
        for (i = 0; !feof(file) && tmp != ']' && (i < 3); ++i) {
          token_op[i] = tmp;
          tmp = fgetc(file);
        }
        token_op[i] = '\0';
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

  stack_t *formula = NULL;
  while (!stack_is_empty(result)) {
    stack_push(&formula, stack_pop(&result));
  }
  return formula;
}

int symbol_to_value(char symbol, char values[]) {
  if (symbol == '0' || symbol == '1') return (symbol - '0');
  symbol = toupper(symbol);
  return (values[symbol - 'A'] - '0');
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

int compute_formula(stack_t *formula, int values, char symbols[]) {
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
        int op1_value = symbol_to_value(op1, symbols_with_values);
        stack_push(&expression, perform_operation(formula_ptr->value, op1_value, 0)+'0');
      }
    }
    formula_ptr = formula_ptr->next;
  }
  if (stack_is_empty(expression)) return -1;
  return symbol_to_value(stack_top(expression), symbols_with_values) + '0';
}

void print_table(FILE *file) {
  char tmp;
  char symbols[] = "00000000000000000000000000"; // Is there symbol ABC...Z?
  short symbols_cnt = 0;
  while (!feof(file)) { // Searching for variables in formula
    fscanf(file, "%c", &tmp);
    if (isalpha(tmp)) {
      tmp = toupper(tmp);
      if (symbols[tmp-'A'] == '0') {
        symbols[tmp-'A'] = '1';
        symbols_cnt++;
      }
    }
  }
  rewind(file);
  if (symbols_cnt == 0) {
    printf("No variables were found\n");
    return;
  }
  // Printing the header
  for (int i = 0; i < 26; ++i) {
    if (symbols[i] == '1') {
      printf(" %c |", i+'A');
    }
  }
  printf(" Result\n");
  stack_t *formula = formula_to_postfix(file);
  // Print the table itself
  // Bit magic for getting some bit from our string (000, 001, 010, ...,
  // 1 << symbols_cnt - 1 (amount of possible prompts to our formula is 2**symbols_cnt))
  for (int i = 0; i < (1 << symbols_cnt); ++i) {
    for (int j = 1; j <= symbols_cnt; ++j) {
      printf(" %d |", (i >> (symbols_cnt - j)) & 1);
    }
    printf(" %c\n", compute_formula(formula, i, symbols));
  }
}

int main(int argc, char *argv[]) {
  FILE *file;
  if (argc != 2) {
    printf("Usage: truth_table <path_to_file>\n");
  } else {
    file = fopen(argv[1], "r");
    if (file == NULL) {
      printf("File could not be opened\n");
    } else {
      print_table(file);
      fclose(file);
    }
  }
  return 0;
}
