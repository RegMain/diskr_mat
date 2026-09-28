#pragma once
#include "formula.c"
#include <stdlib.h>

char* format_string(const char *template, char *A, char *B) {
  if (!template) {
    return NULL;
  }
  size_t len_A = A ? strlen(A) : 0;
  size_t len_B = B ? strlen(B) : 0;

  size_t total_len = 0;
  for (char *p = template; *p; ) {
    if (p[0] == '{' && p[1] == 'A' && p[2] == '}') {
      total_len += len_A;
      p += 3;
    } else if (p[0] == '{' && p[1] == 'B' && p[2] == '}') {
      total_len += len_B;
      p += 3;
    } else {
      total_len++;
      p++;
    }
  }

  char *result = malloc(total_len + 1);
  if (!result) {
    return NULL;
  }

  char *dst = result;
  for (char *p = template; *p;) {
    if (p[0] == '{' && p[1] == 'A' && p[2] == '}') {
      if (len_A > 0) {
        memcpy(dst, A, len_A);
        dst += len_A;
      }
      p += 3;
    } else if (p[0] == '{' && p[1] == 'B' && p[2] == '}') {
      if (len_B > 0) {
        memcpy(dst, B, len_B);
        dst += len_B;
      }
      p += 3;
    } else {
      *dst++ = *p++;
    }
  }
  *dst = '\0';

  return result;
}

/*
  0 - and, or, not
  2 - nand
*/

char *basis_zero(int basis) {
  switch (basis) {
    case 0:
      return "($A [+] [!]$A)";
    case 1:
    case 2:
      return "($A)"
  }
}

char *basis_to_nand(int opcode, char *op1, char *op2) {
  char *result;
  switch (opcode) {
    case 0: // !A = A | A
      result = format_string("({B} | {B})", op1, NULL);
      return result;
    case 1: // A+B = (A | A) | (B | B)
      result = format_string("(({A} [!*] {A}) | ({B} [!*] {B}))", op1, op2);
    case 2: // A*B = (A | B) | (A | B)
      result = format_string("(({A} [!*] {B}) [!*] ({A} [!*] {B}))", op1, op2);
    case 3: // A^B = ((A | B) | A) | ((A | B) | B)
      result = format_string("((({A} [!*] {B}) [!*] {A}) [!*] (({A} [!*] {B}) [!*] {B}))", op1, op2);
    case 4: // A=B = (((A | B) | A) | ((A | B) | B) | ((A | B) | A) | ((A | B) | B))
      result = format_string("(((({A} [!*] {B}) [!*] {A}) [!*] (({A} [!*] {B}) [!*] {B})) [!*] ((({A} [!*] {B}) [!*] {A}) [!*] (({A} [!*] {B}) [!*] {B}))))", op1, op2);
    case 5: // A->B = !A + B = A | (B | B)
      result = format_string("({A} [!*] ({B} [!*] {B}))", op1, op2);
    case 6: // A!->B = A*(!B) = (A | (B | B)) | (A | (B | B))
      result = format_string("(({A} [!*] ({B} [!*] {B})) [!*] ({A} [!*] ({B} [!*] {B})))", op1, op2);
    case 7: // A!+B = ((A | A) | (B | B)) | ((A | A) | (B | B))
      result = format_string("((({A} [*!] {A}) [!*] ({B} [!*] {B})) [!*] (({A} [!*] {A}) [!*] ({B} [!*] {B})))", op1, op2);
    case 8: // A!*B = A | B
      result = format_string("({A} [!*] {B})", op1, op2);
    default:
      result = NULL;
  }
  return result;
}

void basis_change(stack_t *formula, int basis) {
  stack_t *stack = NULL;
  stack_t *formula_ptr = formula;
  char *result = "";
  while (formula_ptr != NULL) {
    if (is_operator(formula_ptr->value)) {
      char *op1 = NULL;
      char *op2 = NULL;
      op2 = stack_pop(&stack);
      if (get_opcode(formula_ptr->value)) {
        op1 = stack_pop(&stack);
      }
      char *result = basis_to_nand(get_opcode(formula_ptr->value), op1, op2);
      free(op2);
      if (!get_opcode(formula_ptr->value)) {
        free(op1);
      }
      stack_push(&stack, result);
    } else {
      if (formula_ptr->value[0] == '0') {
        stack_push(&stack, basis_zero(basis));
      } else if (formula_ptr->value[0] == '1') {
        stack_push(&stack, basis_one(basis));
      }
    }
    formula_ptr = formula_ptr->next;
  }
}

