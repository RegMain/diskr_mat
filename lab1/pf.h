#pragma once
#include <stdio.h>
#include "stack.h"

void pdnf(stack_t *formula, int symbols_cnt, char *symbols);

void pcnf(stack_t *formula, int symbols_cnt, char *symbols);
