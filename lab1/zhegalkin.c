#pragma once
#include <stdio.h>
#include <string.h>

void zhegalkin_polynomial(char* table_of_truth, int symbols_cnt, char *symbols) {
  int flag_none = 1;
  int flag_1 = 0;
  printf("Zhegalkin polynomial: ");
  if (table_of_truth[0] - '0') {
    printf("1");
    flag_1 = 1;
    flag_none = 0;
  }
  int prev, tmp;
  for (int j = 0; j < strlen(table_of_truth) - 1; ++j) {
    prev = 0;
    for (int i = strlen(table_of_truth) - 1 - j; i >= 0; --i) {
      tmp = table_of_truth[i] - '0';
      table_of_truth[i] = ((table_of_truth[i] - '0') ^ prev)+'0';
      prev = tmp;
    }
    if (table_of_truth[0] - '0') {
      if (flag_1) {
        printf(" [^] ");
        flag_1 = 0;
      }
      int flag_first = 1;
      int cnt = 0;
      for (int k = 0; k < symbols_cnt; ++k) {
        for (int c = cnt; c < 26; ++c) {
          if (symbols[c] - '0') {
            cnt = c + 1;
            break;
          }
        }
        if (((j + 1) >> (symbols_cnt - 1 - k)) & 1) {
          if (!flag_first) {
            printf(" [*] ");
          }
          printf("$%c", 'A' + cnt - 1);
          flag_none = 0;
          flag_first = 0;
        }
      }
      flag_1 = 1;
    }
  }
  if (flag_none) {
    printf("0");
  }
  printf("\n");
}
