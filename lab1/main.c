#include <stdio.h>
#include <string.h>
#include "table.c"
#include "pf.c"

int main(int argc, char *argv[]) {
  FILE *file;
  int pdnf_flag = 0;
  int pcnf_flag = 0;
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
      } else {
        printf("Unknown flag: %s\n", argv[i]);
      }
    }
    file = fopen(argv[argc - 1], "r");
    if (file == NULL) {
      printf("File could not be opened\n");
    } else {
      stack_t *formula = print_table(file);
      if (pdnf_flag) pdnf(file, formula);
      if (pcnf_flag) pcnf(file, formula);
      fclose(file);
    }
  }
  return 0;
}
