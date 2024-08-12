#ifndef SCANNER_H
#define SCANNER_H

#include "lox.h"
#include "token_vec.h"
#include "hashmap.h"

typedef struct Scanner Scanner;

struct Scanner {
    const char *source;
    TokenVec vec;
    uint64_t start;
    uint64_t current;
    uint64_t line;
    HashMap keywords;
};

//source is moved into the scanner, the sacnner will not free this memory
Scanner create_scanner(const char *source);
TokenVec scan_tokens(Lox *lox, Scanner *scan);
void delete_scanner(Scanner *scan);

bool scan_is_at_end(Scanner *scan);
void scan_token(Lox *lox, Scanner *scan);
char scan_advance(Scanner *scan); 
void add_token(Scanner *scan, const enum TokenType type);
void add_token_obj(Scanner *scan, const enum TokenType type, const Object literal);
char scan_peek(Scanner *scan);
bool scan_match(Scanner *scan, const char expected);
void string(Lox *lox, Scanner *scan);
bool is_digit(const char c);
void number(Scanner *scan);
char peek_next(Scanner *scan);
bool is_alpha(const char c);
void identifier(Scanner *scan);
bool is_alpha_numeric(const char c);


#endif /* SCANNER_H */