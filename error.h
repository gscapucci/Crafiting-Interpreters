#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include "lox.h"

void error(int line, const char *message);
void report(int line, const char *where, const char *message);

#endif /* ERROR_H */