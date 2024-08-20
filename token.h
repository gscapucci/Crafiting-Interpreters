#ifndef TOKEN_H
#define TOKEN_H

#include "token_type.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "object.h"

typedef struct Token Token;

struct Token {
    enum TokenType type;
    char *lexeme;
    Object literal;
    int line;
};

Token create_token(const enum TokenType type, const char *lexeme, const Object literal, const int line);
void delete_token(Token *token);

//remember to free this memory
char *token_to_string(const Token *token);

#endif /* TOKEN_H */