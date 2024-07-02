#include "../include/error.h"

void error(uint64_t line, const char *message) {
    report(line, "", message);
}

void report(uint64_t line, const char *where, const char *message) {
    printf("[line %ld] Error%s: %s\n", line, where, message);
    fflush(stdout);
    lox.hasError = true;
}
