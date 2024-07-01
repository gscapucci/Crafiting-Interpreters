#ifndef SCANNER_H
#define SCANNER_H

#include "token_vec.h"
#include "error.h"

typedef struct Scanner Scanner;
struct Scanner {
    char *source;
    TokenVec tokens;
    uint64_t start;
    uint64_t current;
    uint64_t line;
};

Scanner create_scanner(const char *source);
void delete_scanner(Scanner scan);

TokenVec scan_tokens(Scanner *scan);

bool is_at_end(Scanner *scan);
void scan_token(Scanner *scan);
char advance(Scanner *scan);
void add_token(Scanner *scan, enum TokenType type);
void add_token_obj(Scanner *scan, enum TokenType type, Object literal);
bool match(Scanner *scan, char expected);
char peek(Scanner *scan);
void string(Scanner *scan);
#endif /* SCANNER_H */