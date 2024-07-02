#include "../include/token.h"

Token* new_token(enum TokenType type, const char *lexeme, Object literal, uint64_t line) {
    Token *tk = malloc(sizeof(Token));
    if(!tk) return NULL;
    *tk = create_token(type, lexeme, literal, line);
    return tk;
}
void free_token(Token *tk) {
    if(!tk) return;
    free(tk);
    tk = NULL;
}

Token create_token(enum TokenType type, const char *lexeme, Object literal, uint64_t line) {
    Token tk;
    if(strlen(lexeme) > 255) {
        fprintf(stderr, "Token exeme is too large");
        exit(1);
    }
    memset(tk.lexeme, 0, 256);
    if(strlen(lexeme) > 0) {
        memcpy(tk.lexeme, lexeme, strlen(lexeme));
    }
    tk.type = type;
    tk.literal = literal;
    tk.line = line;
    return tk;
}
void delete_token(Token tk) {
    (void)tk;
}

char *token_to_str(const Token *token) {
    char *str = malloc(1024 * sizeof(char));
    memset(str, 0, 1024);
    sprintf(str, "%s(\"%s\") ", token_type_to_str[token->type], token->lexeme);
    char *aux = object_to_str(token->literal);
    strcat(str, aux);
    free(aux);
    return str;
}