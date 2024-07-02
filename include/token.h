#ifndef TOKEN_H
#define TOKEN_H

#include "token_type.h"
#include "object.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Token Token;

struct Token {
    enum TokenType type;
    char lexeme[256];
    Object literal;
    int line;
};

Token* new_token(enum TokenType type, const char *lexeme, Object literal, int line);
void free_token(Token *tk);

Token create_token(enum TokenType type, const char *lexeme, Object literal, int line);
void delete_token(Token tk);

char *token_to_str(const Token *token);
#endif /* TOKEN_H */