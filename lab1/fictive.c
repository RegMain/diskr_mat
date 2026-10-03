#include "fictive.h"
#include "stack.h"
#include "formula.h"

void print_fictive(stack_t *formula, int symbols_cnt, char *symbols) {
  int found_fictive = 0;
  int is_fictive;
  int cnt = 0, result_with_0, result_with_1;
  printf("Fictive variables: ");
  for (int i = 0; i < symbols_cnt; ++i) {
    is_fictive = 1;
    for (int j = 0; j < (1 << symbols_cnt - 1); ++j) {
      // If result doesn't depend on variable
      // for all strings then it is fictive.
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
  if (!found_fictive) {
    printf("None");
  }
  printf("\n");
}
