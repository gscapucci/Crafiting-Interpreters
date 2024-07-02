#ifndef LOX_H
#define LOX_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "scanner.h"
#include "expr.h"

typedef struct Lox Lox;

struct Lox {
    int argc;
    char **argv;
    char *file;
    bool hasError;
};

static Lox lox = {0};

char *file_to_str(const char *path);
void lox_main(int argc, char **argv);
void run_file(const char *path);
void run_prompt();
void run(const char *source);

#endif /* LOX_H */