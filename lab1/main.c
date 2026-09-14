#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"
#include "queue.h"

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

short perform_operation(char *op, short a, short b) {
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
    default:
      return -1;
  }
}

int compute_formula(stack_t *formula, int values) {
  // TODO
}

void print_table(FILE *file) {
  char tmp;
  char *symbols = "00000000000000000000000000";
  FILE *cycle_ptr = &(*file);
  while (!feof(cycle_ptr)) {
    fscanf(cycle_ptr, "%c", &tmp);
    if (isalpha(tmp)) {
      tmp = toupper(tmp);
      symbols[tmp-'a'] = '1';
    }
  }
  cycle_ptr = &(*file);
  for (int i = 0; i < 26; ++i) {
    if (symbols[i] == '1') {
      printf(" %c |", i+'A');
    }
  }
  printf(" Result\n");
  // TODO
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
    }
    fclose(file);
  }
  return 0;
}
