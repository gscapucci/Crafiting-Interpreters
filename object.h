#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Object Object;
enum ObjectType {
    ObjectTypeNum,
    ObjectTypeString,
    ObjectTypeNil,
    ObjectTypeTrue,
    ObjectTypeFalse,
    ObjectTypeBool,
    ObjectNone
};

struct Object {
    enum ObjectType type;
    union {
        double num;
        char string[256];
        bool value;
    };
};

char *object_to_str(Object obj);

#endif /* OBJECT_H */