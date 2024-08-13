#include "token.h"

Object create_object_from_int(const int64_t num) {
    Object obj;
    obj.object_type = OBJ_TYPE_INT;
    obj.value.i_num = num;
    return obj;
}

Object create_object_from_double(const double num) {
    Object obj;
    obj.object_type = OBJ_TYPE_FLOAT;
    obj.value.f_num = num;
    return obj;
}

Object create_object_from_str(const char *str) {
    Object obj;
    obj.object_type = OBJ_TYPE_STRING;
    obj.value.str = malloc(strlen(str) + 1);
    memcpy(obj.value.str, str, strlen(str) + 1);
    return obj;
}

Object create_object_from_bool(const bool b) {
    Object obj;
    obj.object_type = OBJ_TYPE_BOOL;
    obj.value.b = b;
    return obj;
}

Object *new_object_from_int(const int64_t num) {
    Object *obj = malloc(sizeof(Object));
    *obj = create_object_from_int(num);
    return obj;
}

Object *new_object_from_double(const double num) {
    Object *obj = malloc(sizeof(Object));
    *obj = create_object_from_double(num);
    return obj;
}

Object *new_object_from_str(const char *str) {
    Object *obj = malloc(sizeof(Object));
    *obj = create_object_from_str(str);
    return obj;
}

Object *new_object_from_bool(const bool b) {
    Object *obj = malloc(sizeof(Object));
    *obj = create_object_from_bool(b);
    return obj;
}

char *object_to_string(const Object *object) {
    char *ret = calloc(100, 1);
    switch(object->object_type) {
        case OBJ_TYPE_FLOAT:
            sprintf(ret, "%lf", object->value.f_num);
            break;
        case OBJ_TYPE_INT:
            sprintf(ret, "%ld", object->value.i_num);
            break;
        case OBJ_TYPE_STRING:
            strcpy(ret, object->value.str);
            break;
        case OBJ_TYPE_NONE:
            strcpy(ret, "NONE");
            break;
        case OBJ_TYPE_NULL:
            strcpy(ret, "NULL");
            break;
        default:
            printf("%d\n", (int)object->object_type);
            strcpy(ret, "Unkown Type");
            break;
    }
    return ret;
}

int object_compare(const Object left, const Object right) {
    if(left.object_type == OBJ_TYPE_STRING && right.object_type == OBJ_TYPE_STRING) {
        return strcmp(left.value.str, right.value.str);
    }
    if(left.object_type == OBJ_TYPE_FLOAT && right.object_type == OBJ_TYPE_FLOAT) {
        return left.value.f_num == right.value.f_num ? 0 : (left.value.f_num > right.value.f_num ? 1 : -1);
    }
    if(left.object_type == OBJ_TYPE_INT && right.object_type == OBJ_TYPE_INT) {
        return left.value.i_num - right.value.i_num;
    }
    if(left.object_type == OBJ_TYPE_BOOL && right.object_type == OBJ_TYPE_BOOL) {
        return left.value.b - left.value.b;
    }
    if( (left.object_type == OBJ_TYPE_NONE || left.object_type == OBJ_TYPE_NULL) &&
        (right.object_type == OBJ_TYPE_NONE || right.object_type == OBJ_TYPE_NULL)) {
        return 0;
    }
    return -1;
}

void delete_object(Object *object) {
    if(!object) return;
    if(object->object_type == OBJ_TYPE_STRING && object->value.str != NULL) {
        free(object->value.str);
    }
}
void free_object(Object *object) {
    if(!object) return;
    if(object->object_type == OBJ_TYPE_STRING && object->value.str != NULL) {
        free(object->value.str);
    }
    free(object);
}

Object copy_object(const Object *object) {
    if(!object) return OBJECT_NONE();
    if(object->object_type != OBJ_TYPE_STRING) {
        return *object;
    }
    Object copy;
    copy.object_type = OBJ_TYPE_STRING;
    copy.value.str = malloc(strlen(object->value.str) + 1);
    memcpy(copy.value.str, object->value.str, strlen(object->value.str) + 1);
    return copy;
}

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

Object object_minus(Object left, Object right) {
    if(left.object_type == OBJ_TYPE_INT) {
        left.value.i_num -= right.value.i_num;
        return left;
    }
    if(left.object_type == OBJ_TYPE_FLOAT) {
        left.value.f_num -= right.value.f_num;
        return left;
    }
    fprintf(stderr, "Unreachable.");
    fflush(stdout);
    exit(1);
}

Object object_divide(Object left, Object right) {
    if(left.object_type == OBJ_TYPE_INT) {
        left.value.i_num /= right.value.i_num;
        return left;
    }
    if(left.object_type == OBJ_TYPE_FLOAT) {
        left.value.f_num /= right.value.f_num;
        return left;
    }
    fprintf(stderr, "Unreachable.");
    fflush(stdout);
    exit(1);
}

Object object_multiply(Object left, Object right) {
    if(left.object_type == OBJ_TYPE_INT) {
        left.value.i_num *= right.value.i_num;
        return left;
    }
    if(left.object_type == OBJ_TYPE_FLOAT) {
        left.value.f_num *= right.value.f_num;
        return left;
    }
    
    fprintf(stderr, "Unreachable.");
    fflush(stdout);
    exit(1);
}