#include "error.h"

void error_token(Lox *lox, Token token, const char *message) {
    if(token.type == EEOF) {
        report(lox, token.line, " at end", message);
    } else {
        char aux[100] = {0};
        strcat(aux, " at '");
        strcat(aux, token.lexeme);
        strcat(aux, "'");
        report(lox, token.line, aux, message);
    }
}

void error(Lox *lox, int line, const char *message) {
    report(lox, line, "", message);
}

void runtime_error(Lox *lox, int line, const char *message) {
    printf("%s [Line %d]\n", message, line);
    lox->had_runtime_error = true;
}
void report(Lox *lox, int line, const char *where, const char *message) {
    printf("[Line %d] Error %s: %s\n", line, where, message);
    fflush(stdout);
    lox->had_error = true;
}

char *error_as_str(Lox *lox, int line, const char *message) {
    return report_as_str(lox, line, "", message);
}

char *report_as_str(Lox *lox, int line, const char *where, const char *message) {
    char *ret = calloc(strlen(where) + strlen(message) + 150, 1);
    sprintf(ret, "[Line %d] Error %s: %s", line, where, message);
    lox->had_error = true;
    return ret;
}

char *error_token_as_str(Lox *lox, Token token, const char *message) {
    if(token.type == EEOF) {
        return report_as_str(lox, token.line, " at end", message);
    }
    char aux[100] = {0};
    strcat(aux, " at '");
    strcat(aux, token.lexeme);
    strcat(aux, "'");
    return report_as_str(lox, token.line, aux, message);
}