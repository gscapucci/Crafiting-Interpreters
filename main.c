#include "lox.h"

int main(int argc, char **argv) {
    Lox *lox = new_lox(argc, argv);
    int ret = lox_main(lox);
    free_lox(lox);
    return ret;
}