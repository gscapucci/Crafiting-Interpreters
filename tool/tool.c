#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define WRITE_LINE(file, fmt, ...)

char *str_to_lowercase(const char *str) {
    char *res = malloc(strlen(str) + 1);
    for(uint64_t i = 0; i < strlen(str); i++) {
        res[i] = tolower(str[i]);
    }
    res[strlen(str)] = '\0';
    return res;
}

char *str_to_uppercase(const char *str) {
    char *res = malloc(strlen(str) + 1);
    for(uint64_t i = 0; i < strlen(str); i++) {
        res[i] = toupper(str[i]);
    }
    res[strlen(str)] = '\0';
    return res;
}

void define_ast(const char *base_name, const char **types) {
    char header_path[256] = {0};
    char src_path[256] = {0};

    char *base_name_lower = str_to_lowercase(base_name);
    char *base_name_upper = str_to_uppercase(base_name);
    
    sprintf(header_path, "../include/%s.h", base_name_lower);
    sprintf(src_path, "../src/%s.c", base_name_lower);

    FILE *header_file = fopen(header_path, "w");
    FILE *rsc_file = fopen(src_path, "w");

    write_line(header_file, "#ifndef %s_H\n", base_name_upper);

    char line[256] = {0};
    sprint(line, "#ifndef %s_H\n", base_name_upper);
    fwrite(line, 1, strlen(line), header_file);
    memset(line, 0, 256);

    free(base_name_lower);
    free(base_name_upper);
    fclose(header_file);
    fclose(rsc_file);
}

int main(int argc, char **argv) {
    define_ast("Expr", (char **){
        "Binary     : Expr left, Token operator, Expr right",
        "Grouping   : Expr expression",
        "Literal    : Object value",
        "Unary      : Token operator, Expr right"
    });
    return 0;
}