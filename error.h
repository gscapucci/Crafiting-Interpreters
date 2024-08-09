#ifndef ERROR_H
#define ERROR_H

#include "lox.h"
#include "token.h"

void error_token(Lox *lox, Token token, const char *message);
void error(Lox *lox, int line, const char *message);
void report(Lox *lox, int line, const char *where, const char *message);
// remembert to free the return
char *error_token_as_str(Lox *lox, Token token, const char *message);
// remembert to free the return
char *report_as_str(Lox *lox, int line, const char *where, const char *message);
// remembert to free the return

#endif /* ERROR_H */