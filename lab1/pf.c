#include "pf.h"
#include <stdio.h>
#include "formula.h"

void pdnf(stack_t *formula, int symbols_cnt, char *symbols) {
  printf("PDNF: ");
  int first_flag = 1;
  for (int i = 0; i < (1 << symbols_cnt); ++i) {
    if (compute_formula(formula, i, symbols) - '0') {
      // If value in formula is 1 then we print
      // Multiplication of values (maybe their negatives)
      if (first_flag) {
        first_flag = 0;
      } else {
        printf(" [+] ");
      }
      int cnt = 0;
      for (int j = 1; j <= symbols_cnt; ++j) {
        for (int k = cnt; k < 26; ++k) {
          if (symbols[k] - '0') {
            cnt = k + 1;
            break;
          }
        }
        if (!((i >> (symbols_cnt - j)) & 1)) {
           printf("[!]");
        }
        printf("$%c", 'A' + cnt - 1);
        if (j != symbols_cnt) {
          printf(" [*] ");
        }
      }
    } 
  }
  if (first_flag) {
    printf("No opportunity to construct a PDNF because of function is always 0");
  } else if (symbols_cnt == 0) {
    printf("$1");
  }
  printf("\n");
}

void pcnf(stack_t *formula, int symbols_cnt, char *symbols) {
  printf("PCNF: ");
  int first_flag = 1;
  for (int i = 0; i < (1 << symbols_cnt); ++i) {
    if (!(compute_formula(formula, i, symbols) - '0')) {
      // If value in formula is 0 then we print
      // Sum of values (or maybe their negatives)
      if (first_flag) {
        first_flag = 0;
      } else {
        printf(" [*] ");
      }
      printf("(");
      int cnt = 0;
      for (int j = 1; j <= symbols_cnt; ++j) {
        for (int k = cnt; k < 26; ++k) {
          if (symbols[k] - '0') {
            cnt = k + 1;
            break;
          }
        }
        if ((i >> (symbols_cnt - j)) & 1) {
           printf("[!]");
        }
        printf("$%c", 'A' + cnt - 1);
        if (j != symbols_cnt) {
          printf(" [+] ");
        } else {
          printf(")");
        }
      }
    } 
  }
  if (first_flag) {
    printf("No opportunity to construct a PCNF because of function is always 1");
  } else if (symbols_cnt == 0) {
    printf("$0)");
  }
  printf("\n");
}


