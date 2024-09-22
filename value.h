#ifndef VALUE_H
#define VALUE_H

#include "common.h"

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_FLOAT,
    VAL_INT,
    VAL_UINT,
    VAL_OBJ
} ValueType;

typedef struct {
    ValueType type;
    union {
        uint64_t uint_number;
        bool boolean;
        double float_number;
        int64_t int_number;
        Obj *obj;
    } as;
} Value;

static inline bool _is_number(Value value) {
    return value.type == VAL_FLOAT || value.type == VAL_INT || value.type == VAL_UINT;
}

#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NIL(value)       ((value).type == VAL_NIL)
#define IS_NUMBER(value)    (_is_number(value))
#define IS_FLOAT(value)     ((value).type == VAL_FLOAT)
#define IS_INT(value)       ((value).type == VAL_INT)
#define IS_UINT(value)      ((value).type == VAL_UINT)
#define IS_OBJ(value)       ((value).type == VAL_OBJ)

#define AS_OBJ(value)       ((value).as.obj)
#define AS_BOOL(value)      ((value).as.boolean)
#define AS_FLOAT(value)     ((value).as.float_number)
#define AS_INT(value)       ((value).as.int_number)
#define AS_UINT(value)      ((value).as.uint_number)

#define BOOL_VAL(value)     ((Value){.type = VAL_BOOL, .as = {.boolean = value}})
#define NIL_VAL             ((Value){.type = VAL_NIL, .as = {0}})
#define FLOAT_VAL(value)    ((Value){.type = VAL_FLOAT, .as = {.float_number = value}})
#define INT_VAL(value)      ((Value){.type = VAL_INT, .as = {.int_number = value}})
#define UINT_VAL(value)     ((Value){.type = VAL_UINT, .as = {.uint_number = value}})
#define OBJ_VAL(object)     ((Value){.type = VAL_OBJ, .as = {.obj = (Obj*)object}})

typedef struct {
    uint64_t capacity;
    uint64_t count;
    Value *values;
} ValueArray;

bool values_equal(Value a, Value b);
void init_value_array(ValueArray *array);
void write_value_array(ValueArray *array, Value value);
void free_value_array(ValueArray *array);
void print_value(Value value);

#endif /* VALUE_H */