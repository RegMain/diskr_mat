#pragma once

#include <stdio.h>
#include <ctype.h>
#include "formula.c"

void pdnf(FILE *file, stack_t *formula) {
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
  printf("PDNF: ");
  int first_flag = 1;
  for (int i = 0; i < (1 << symbols_cnt); ++i) {
    if (compute_formula(formula, i, symbols) - '0') {
      if (first_flag) {
        first_flag = 0;
      } else {
        printf(" [+] ");
      }
      int cnt = 0;
      int index = 0;
      for (int j = 1; j <= symbols_cnt; ++j) {
        for (int k = cnt; k < 26; ++k) {
          cnt += (symbols[k] == '1');
          ++index;
          if (cnt == j) break;
        }
        if (!((i >> (symbols_cnt - j)) & 1)) {
           printf("[!]");
        }
        printf("$%c", 'A' + index - 1);
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

void pcnf(FILE *file, stack_t *formula) {
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
  printf("PCNF: ");
  rewind(file);
  int first_flag = 1;
  for (int i = 0; i < (1 << symbols_cnt); ++i) {
    if (!(compute_formula(formula, i, symbols) - '0')) {
      if (first_flag) {
        first_flag = 0;
      } else {
        printf(" [*] ");
      }
      printf("(");
      int cnt = 0;
      int index = 0;
      for (int j = 1; j <= symbols_cnt; ++j) {
        for (int k = cnt; k < 26; ++k) {
          cnt += (symbols[k] == '1');
          ++index;
          if (cnt == j) break;
        }
        if ((i >> (symbols_cnt - j)) & 1) {
           printf("[!]");
        }
        printf("$%c", 'A' + index - 1);
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


