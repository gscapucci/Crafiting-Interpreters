#include "object.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    char *aux = escape_str(str);
    obj.object_type = OBJ_TYPE_STRING;
    obj.value.str = malloc(strlen(aux) + 1);
    memcpy(obj.value.str, aux, strlen(aux) + 1);
    free(aux);
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

char* escape_str(const char* input) {
    // Calcula o tamanho necessário para a string desscapada
    size_t tamanho = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (input[i] == '\\') {
            switch (input[i + 1]) {
                case 't':
                case 'r':
                case 'n':
                case '"':
                case '\'':
                case '\\':
                    tamanho++;
                    i++; // Pula o próximo caractere já que ele foi escapado
                    break;
                default:
                    tamanho++;
                    break;
            }
        } else {
            tamanho++;
        }
    }

    // Aloca memória para a nova string
    char* output = (char*)malloc((tamanho + 1) * sizeof(char));
    if (output == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }

    // Copia e desscapa os caracteres
    size_t j = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (input[i] == '\\') {
            switch (input[i + 1]) {
                case 't':
                    output[j++] = '\t';
                    i++;
                    break;
                case 'r':
                    output[j++] = '\r';
                    i++;
                    break;
                case 'n':
                    output[j++] = '\n';
                    i++;
                    break;
                case '"':
                    output[j++] = '"';
                    i++;
                    break;
                case '\\':
                    output[j++] = '\\';
                    i++;
                    break;
                case '\'':
                    output[j++] = '\'';
                    i++;
                    break;
                default:
                    output[j++] = input[i];
                    break;
            }
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0'; // Termina a string

    return output;
}
