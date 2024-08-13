#ifndef LOX_H
#define LOX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


typedef struct Lox Lox;

struct Lox {
    int argc;
    char **argv;
    bool had_error;
    bool had_runtime_error;
};

Lox *new_lox(int argc, char **argv);
void free_lox(Lox *lox);

int lox_main(Lox *lox);
int run_file(Lox *lox);
int run_prompt(Lox *lox);
int run(Lox *lox, char *source);

//remember to free this memory
char *read_file(const char *path);

#endif /* LOX_H */