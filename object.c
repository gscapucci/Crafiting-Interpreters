#include "object.h"

char *object_to_str(Object obj) {
    char *str = malloc(256 * sizeof(char));
    memset(str, 0, 256);
    switch (obj.type) {
        case ObjectTypeNum:
            sprintf(str, "%f", obj.num);
            break;
        case ObjectTypeString:
            strcpy(str, obj.string);
            break;
        case ObjectTypeNil:
            strcpy(str, "Nil");
            break;
        case ObjectTypeTrue:
            strcpy(str, "True");
            break;
        case ObjectTypeFalse:
            strcpy(str, "False");
            break;
        case ObjectTypeBool:
            sprintf(str, "%s", obj.value ? "True" : "False");
            break;
        case ObjectNone:
            strcpy(str, "NONE");
            break;
    }
    return str;
}