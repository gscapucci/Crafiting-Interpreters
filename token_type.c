#include "token_type.h"

char *token_type_to_string(enum TokenType tt) {
    switch(tt) {
        case LEFT_PAREN:
            return "LEFT_PAREN";
        case RIGHT_PAREN:
            return "RIGHT_PAREN";
        case LEFT_BRACE:
            return "LEFT_BRACE";
        case RIGHT_BRACE:
            return "RIGHT_BRACE";
        case COMMA:
            return "COMMA";
        case DOT:
            return "DOT";
        case MINUS:
            return "MINUS";
        case PLUS:
            return "PLUS";
        case SEMICOLON:
            return "SEMICOLON";
        case SLASH:
            return "SLASH";
        case STAR:
            return "STAR";
        case BANG:
            return "BANG";
        case BANG_EQUAL:
            return "BANG_EQUAL";
        case EQUAL:
            return "EQUAL";
        case EQUAL_EQUAL:
            return "EQUAL_EQUAL";
        case GREATER:
            return "GREATER";
        case GREATER_EQUAL:
            return "GREATER_EQUAL";
        case LESS:
            return "LESS";
        case LESS_EQUAL:
            return "LESS_EQUAL";
        case IDENTIFIER:
            return "IDENTIFIER";
        case STRING:
            return "STRING";
        case INT:
            return "INT";
        case FLOAT:
            return "FLOAT";
        case AND:
            return "AND";
        case CLASS:
            return "CLASS";
        case ELSE:
            return "ELSE";
        case FALSE:
            return "FALSE";
        case FUN:
            return "FUN";
        case FOR:
            return "FOR";
        case IF:
            return "IF";
        case NIL:
            return "NIL";
        case OR:
            return "OR";
        case PRINT:
            return "PRINT";
        case RETURN:
            return "RETURN";
        case SUPER:
            return "SUPER";
        case THIS:
            return "THIS";
        case TRUE:
            return "TRUE";
        case VAR:
            return "VAR";
        case WHILE:
            return "WHILE";
        case EEOF:
            return "EEOF";
        default:
            return NULL;
    }
}


int64_t compare_tokentype(void *key1, void *key2) {
    if(!key1) return -1;
    if(!key2) return -2;

    return *(enum TokenType*)key1 == *(enum TokenType*)key2 ? 0 : 1;
}

void copy_tokentype(void **dst, void *src) {
    if(*dst == NULL) {
        *dst = malloc(sizeof(enum TokenType));
    }
    memcpy(*dst, src, sizeof(enum TokenType));
}