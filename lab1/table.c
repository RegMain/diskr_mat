#pragma once

#include "formula.c"

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
  stack_t *formula = formula_to_postfix(file);
  rewind(file);
  if (symbols_cnt == 0) {
    printf("The result of formula is always %c\n", compute_formula(formula, 0, symbols));
    return;
  }
  // Printing the header
  for (int i = 0; i < 26; ++i) {
    if (symbols[i] == '1') {
      printf(" %c |", i+'A');
    }
  }
  printf(" Result\n");
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
