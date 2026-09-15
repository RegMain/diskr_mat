#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"

short check_priority(char *op) {
  if (!strcmp(op, "!")) return 6;
  if (!strcmp(op, "*") || !strcmp(op, "!*")) return 5;
  if (!strcmp(op, "^")) return 4;
  if (!strcmp(op, "+") || !strcmp(op, "!+")) return 3;
  if (!strcmp(op, "->") || !strcmp(op, "!->")) return 2;
  if (!strcmp(op, "!^")) return 1;
  return 0;
}

int is_operator(char *ch) {
  return check_priority(ch);
} 

stack_t* formula_to_postfix(FILE *file) {
  stack_t *stack = NULL;
  stack_t *result = NULL;
  // TODO
  while (!feof(file)) {
    // Их нужно поправить
    int type = fgetc(file);
    int tmp;
    switch (type) {
    case '$':
      char *token_ch = malloc(2);
      tmp = fgetc(file);
      token_ch[0] = tmp;
      token_ch[1] = '\0';
      stack_push(&result, token_ch);
      break;
    case '[': {
      int i;
      char *token_op = malloc(4);
      tmp = fgetc(file);
      for (i = 0; !feof(file) && tmp != ']' && (i < 3); ++i) {
        token_op[i] = tmp;
        tmp = fgetc(file);
      }
      token_op[i] = '\0';
      while (!stack_is_empty(stack)
            && ((check_priority(stack_top(stack)) >= check_priority(token_op)))) {
        stack_push(&result, stack_pop(&stack));
      }
      stack_push(&stack, token_op);
      break;
    }
    case '(':
      char *token_br = malloc(2);
      token_br[0] = '(';
      token_br[1] = '\0';
      stack_push(&stack, token_br);
      break;
    case ')':
      while (!stack_is_empty(stack) && strcmp(stack_top(stack), "(")) {
        stack_push(&result, stack_pop(&stack));
      }
      if (!stack_is_empty(stack) && !strcmp(stack_top(stack),"(")) {
        char *to_free = stack_pop(&stack);
        free(to_free);
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
  
}

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

char* perform_operation(char *op, int a, int b) {
  short opcode = get_opcode(op);
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

int compute_formula(stack_t *formula, int values, char symbols[]) {
  char symbols_with_values[26];
  strcpy(symbols_with_values, symbols);
  for (int i = 26; i >= 0; --i) {
    if (symbols_with_values[i] - '0') {
      symbols_with_values[i] = '0' + (values % 2);
      values /= 2;
    }
  }
  stack_t *stack = stack_copy(formula);
  stack_t *expression = NULL;
  while (!stack_is_empty(stack)) {
    if (!is_operator(stack_top(stack))) {
      stack_push(&expression, stack_pop(&stack));
    } else {
      char op1 = stack_pop(&expression)[0];
      char op2 = 0;
      if (!strcmp(stack_top(stack), "!")) {
        char op2 = stack_pop(&expression)[0]; 
      }
      int op1_value
      stack_push(&expression, perform_operation(stack_top(stack), op1, op2));
    }
  }
  stack_free(&stack);
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
    printf(" %d\n", compute_formula(formula, i, symbols));
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
      //print_table(file);
      fclose(file);
    }
  }
  return 0;
}
