#include "token_vec.h"

TokenVec create_token_vec(uint64_t start_cap) {
    TokenVec tv;
    tv.cap = start_cap;
    tv.size = 0;
    tv.tokens = malloc(tv.cap * sizeof(Token));
    return tv;
}

void delete_token_vec(TokenVec token_vec) {
    if(token_vec.tokens) {
        free(token_vec.tokens);
        token_vec.tokens = NULL;
    }
}

Token* token_vec_at(TokenVec *token_vec, uint64_t index) {
    if(!token_vec) return NULL;
    if(index >= token_vec->size) return NULL;
    return &((*token_vec).tokens[index]);
}

void token_vec_push(TokenVec *token_vec, Token token) {
    if(token_vec->size >= token_vec->cap) {
        token_vec->cap *= 2;
        token_vec->tokens = realloc(token_vec->tokens, token_vec->cap * sizeof(Token));
    }
    token_vec->tokens[token_vec->size] = token;
    token_vec->size++;
}