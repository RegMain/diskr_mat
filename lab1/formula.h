#pragma once
#include "stack.h"
#include <stdio.h>

short get_opcode(char *op);

char perform_operation(int opcode, int a, int b);

int symbol_to_value(char *symbol, char values[]);

int is_operator(char *op);

short check_priority(int opcode);

stack_t* formula_to_postfix(FILE *file);

int compute_formula(stack_t *formula, int values, char *symbols);
