#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "formula.h"
#include "table.h"
#include "pf.h"
#include "fictive.h"
#include "basis.h"
#include "zhegalkin.h"

int main(int argc, char *argv[]) {
  FILE *file;
  int pdnf_flag = 0;
  int pcnf_flag = 0;
  int fictive_flag = 0;
  int diff_basis_flag = 0;
  if (argc < 2) {
    printf("Usage: truth_table [--flags] <path_to_file>\n");
  } else {
    for (int i = 1; i < argc - 1; ++i) {
      if (strlen(argv[i]) < 2 || argv[i][0] != '-' || argv[i][1] != '-') {
        printf("Unknown parameter: %s\n", argv[i]);
        return 1;
      }
      if (!strcmp(argv[i], "--pdnf")) {
        pdnf_flag = 1;
      } else if (!strcmp(argv[i], "--pcnf")) {
        pcnf_flag = 1;
      } else if (!strcmp(argv[i], "--fictive")) {
        fictive_flag = 1;
      } else if (!strcmp(argv[i], "--different_basis")) {
        diff_basis_flag = 1;
      } else {
        printf("Unknown flag: %s\n", argv[i]);
      }
    }
    file = fopen(argv[argc - 1], "r");
    if (file == NULL) {
      printf("File could not be opened\n");
    } else {
      stack_t *formula = formula_to_postfix(file);
      int symbols_cnt;
      char symbols[26];
      char *table_of_truth = print_table(file, formula, &symbols_cnt, symbols);
      if (pdnf_flag) pdnf(formula, symbols_cnt, symbols);
      if (pcnf_flag) pcnf(formula, symbols_cnt, symbols);
      if (fictive_flag) print_fictive(formula, symbols_cnt, symbols);
      if (diff_basis_flag) {
        for (int i = 0; i < 3; ++i) {
          basis_change(formula, i);
        }
        zhegalkin_polynomial(table_of_truth, symbols_cnt, symbols);
      }
      fclose(file);
      free(table_of_truth);
      while (!stack_is_empty(formula)) {
        char *to_free = stack_pop(&formula);
        free(to_free);
      }
    }
  }
  return 0;
}
