#ifndef TOKEN_VEC_H
#define TOKEN_VEC_H

#include <stdlib.h>
#include <stdint.h>

#include "token.h"

typedef struct TokenVec TokenVec;

struct TokenVec {
    Token *tokens;
    uint64_t size;
    uint64_t cap;
};

TokenVec create_token_vec(uint64_t start_cap);
void delete_token_vec(TokenVec token_vec);
Token* token_vec_at(TokenVec *token_vec, uint64_t index);
void token_vec_push(TokenVec *token_vec, Token token);
#endif /* TOKEN_VEC_H */