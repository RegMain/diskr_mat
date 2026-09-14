#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"
#include "queue.h"

int is_higher_priority(char *a, char *b) {
  
}

queue_t validate_formula(FILE *file) {
  stack_t *stack;
  queue_t queue;
  char type;
  char *tmp;
  while (!feof(file)) {
    fscanf(file, "%c", &type);
    switch (type) {
    case '$':
      fscanf(file, "%c", tmp);
      queue_push(queue, *tmp);
      break;
    case '#':
      fscanf(file, "%s", tmp);
      if (stack_is_empty(&stack) || !strcmp(stack_top(&stack), "(")) {
        stack_push(&stack, tmp);    
      } else if (is_higher_priority(tmp, stack_top(&stack))) {
        
      }
    default:
      break;
    }
  }
  return queue;
}

int compute_formula(queue_t formula, int values) {
  return 1;
  // TODO
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
  queue_t formula = validate_formula(file);
  // Print the table itself
  // Bit magic for getting some bit from our string (000, 001, 010, ...,
  // 1 << symbols_cnt - 1 (amount of possible prompts to our formula is 2**symbols_cnt))
  for (int i = 0; i < (1 << symbols_cnt); ++i) {
    for (int j = 1; j <= symbols_cnt; ++j) {
      printf(" %d |", (i >> (symbols_cnt - j)) & 1);
    }
    printf(" %d\n", compute_formula(formula, i));
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
