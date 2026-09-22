#pragma once

#include "formula.c"

void print_fictive(FILE *file, stack_t *formula) {
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
  int found_fictive = 0;
  int is_fictive;
  int cnt = 0, result_with_0, result_with_1;
  printf("Fictive variables: ");
  for (int i = 0; i < symbols_cnt; ++i) {
    is_fictive = 1;
    for (int j = 0; j < (1 << symbols_cnt - 1); ++j) {
      if (i == 0) {
        result_with_0 = compute_formula(formula, j, symbols);
        result_with_1 = compute_formula(formula, (1 << symbols_cnt - 1) + j, symbols);
      } else if (i + 1 == symbols_cnt) {
        result_with_0 = compute_formula(formula, (j << 1), symbols);
        result_with_1 = compute_formula(formula, (j << 1) + 1, symbols);
      } else {
        result_with_0 = compute_formula(formula, ((j >> symbols_cnt - i - 1) << (symbols_cnt - i)) + j % (1 << symbols_cnt - i - 1), symbols);
        result_with_1 = compute_formula(formula, ((j >> symbols_cnt - i - 1) << (symbols_cnt - i)) + (1 << symbols_cnt - i - 1) + j % (1 << symbols_cnt - i - 1), symbols);
      }
      if (result_with_0 != result_with_1) {
        is_fictive = 0;
        break;
      }
    }
    for (int j = cnt; j < 26; ++j) {
      if (symbols[j] - '0') {
        cnt = j+1;
        break;
      }
    }
    if (is_fictive) {
      printf("%c ", 'A'+cnt-1);
      found_fictive = 1;
    }
  }
  if (found_fictive) {
    printf("None");
  }
  printf("\n");
}
