#include "token_vec.h"

TokenVec create_token_vec() {
    TokenVec vec;
    vec.tokens = malloc(200 * sizeof(Token));
    vec.size = 0;
    vec.cap = 200;
    return vec;
}

void token_vec_push(TokenVec *vec, Token token) {
    if(!vec) return;
    if(vec->size >= vec->cap) {
        vec->cap *= 2;
        vec->tokens = realloc(vec->tokens, vec->cap);
    }
    vec->tokens[vec->size++] = token;
}
void delete_token_vec(TokenVec *vec) {
    if(!vec) return;
    if(!vec->tokens) return;
    for(uint64_t i = 0; i < vec->size; i++) {
        delete_token(&vec->tokens[i]);
    }
    free(vec->tokens);
}