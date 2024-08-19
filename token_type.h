#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>


enum TokenType {
    UNKNOWN,
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, FLOAT, INT,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    EEOF
};

//you don't need to free this memory
char *token_type_to_string(enum TokenType tt);

int64_t compare_tokentype(void *key1, void *key2);

void copy_tokentype(void **dst, void *src);
#endif /* TOKEN_TYPE_H */