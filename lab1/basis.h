#pragma once
#include "stack.h"

char* format_string(const char *fmt, const char *A, const char *B);

char *basis_zero(int basis);

char *basis_one(int basis);

char *basis_to_and_or(int opcode, char *op1, char *op2);

char *basis_to_nor(int opcode, char *op1, char *op2);

char *basis_to_nand(int opcode, char *op1, char *op2);

void basis_change(stack_t *formula, int basis);
