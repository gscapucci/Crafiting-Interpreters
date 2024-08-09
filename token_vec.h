#ifndef TOKEN_VEC_H
#define TOKEN_VEC_H

#include "token.h"

typedef struct TokenVec TokenVec;

struct TokenVec {
    Token *tokens;
    uint64_t size;
    uint64_t cap;
};

TokenVec create_token_vec();

//move the token
void token_vec_push(TokenVec *vec, Token token);

void delete_token_vec(TokenVec *vec);


#endif /* TOKEN_VEC_H */