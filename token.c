#include "token.h"


Token create_token(const enum TokenType type, const char *lexeme, const Object literal, const int line) {
    Token token;
    token.type = type;
    token.lexeme = malloc(strlen(lexeme) + 1);
    memcpy(token.lexeme, lexeme, strlen(lexeme) + 1);
    token.literal = copy_object(&literal);
    token.line = line;
    return token;
}

void delete_token(Token *token) {
    if(!token) return;
    if(token->lexeme) free(token->lexeme);
    delete_object(&token->literal);
}

char *token_to_string(const Token *token) {
    char *ret = calloc(100, 1);
    char *token_type_str = token_type_to_string(token->type);
    strcat(ret, token_type_str);
    strcat(ret, " ");
    strcat(ret, token->lexeme);
    strcat(ret, " ");
    char *object_str = object_to_string(&token->literal);
    strcat(ret, object_str);
    free(object_str);
    return ret;
}