#ifndef VALUE_H
#define VALUE_H

#include "common.h"

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NIL(value)       ((value).type == VAL_NIL)
#define IS_NUMBER(value)    ((value).type == VAL_NUMBER)

#define AS_BOOL(value)      ((value).as.boolean)
#define AS_NUMBER(value)    ((value).as.number)

#define BOOL_VAL(value)     ((Value){.type = VAL_BOOL, .as = {.boolean = value}})
#define NIL_VAL             ((Value){.type = VAL_NIL, .as = {0}})
#define NUMBER_VAL(value)   ((Value){.type = VAL_NUMBER, .as = {.number = value}})

typedef struct {
    uint64_t capacity;
    uint64_t count;
    Value *values;
} ValueArray;

void init_value_array(ValueArray *array);
void write_value_array(ValueArray *array, Value value);
void free_value_array(ValueArray *array);
void print_value(Value value);

#endif /* VALUE_H */