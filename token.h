#ifndef TOKEN_H
#define TOKEN_H

#include "token_type.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Token Token;
typedef struct Object Object;

enum ObjectType {
    OBJ_TYPE_NONE = 0,
    OBJ_TYPE_NULL,
    OBJ_TYPE_INT,
    OBJ_TYPE_FLOAT,
    OBJ_TYPE_STRING,
    OBJ_TYPE_BOOL,
};

struct Object {
    enum ObjectType object_type;
    union {
        int64_t i_num;
        double f_num;
        char *str;
        bool b;
    } value;
};


struct Token {
    enum TokenType type;
    char *lexeme;
    Object literal;
    int line;
};

#define OBJECT_NONE() (Object){.object_type = OBJ_TYPE_NONE, .value = {0}}
#define NEW_OBJECT_NONE() (Object*)calloc(sizeof(Object), 1)
#define NEW_OBJECT_NIL() (Object*)calloc(sizeof(Object), 1)

Object create_object_from_int(const int64_t num);
Object create_object_from_double(const double num);
Object create_object_from_str(const char *str);
Object create_object_from_bool(const bool b);

Object *new_object_from_int(const int64_t num);
Object *new_object_from_double(const double num);
Object *new_object_from_str(const char *str);
Object *new_object_from_bool(const bool b);

//remember to free this memory
char *object_to_string(const Object *object);
int object_compare(const Object left, const Object right);

void delete_object(Object *object);
void free_object(Object *object);

Object copy_object(const Object *object);

Token create_token(const enum TokenType type, const char *lexeme, const Object literal, const int line);
void delete_token(Token *token);

//remember to free this memory
char *token_to_string(const Token *token);

Object object_minus(Object left, Object right);
Object object_divide(Object left, Object right);
Object object_multiply(Object left, Object right);

#endif /* TOKEN_H */