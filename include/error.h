#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include "lox.h"

void error(uint64_t line, const char *message);
void report(uint64_t line, const char *where, const char *message);

#endif /* ERROR_H */