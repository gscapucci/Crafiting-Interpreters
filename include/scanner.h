#ifndef SCANNER_H
#define SCANNER_H

#include "token_vec.h"
#include "error.h"
#include "hashmap.h"

typedef struct Scanner Scanner;
struct Scanner {
    char *source;
    TokenVec tokens;
    uint64_t start;
    uint64_t current;
    uint64_t line;

    HashMap keywords;
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
void number(Scanner *scan);
char peek_next(Scanner *scan);
void identifier(Scanner *scan);
bool is_digit(char c);
bool is_alpha(char c);
bool is_alphanumeric(char c);
#endif /* SCANNER_H */