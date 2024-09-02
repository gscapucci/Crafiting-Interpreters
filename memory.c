#include <stdlib.h>

#include "memory.h"

void *reallocate(void *pointer, uint64_t old_size, uint64_t new_size) {
    if(new_size == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, new_size);
    if(result == NULL) exit(1);
    return result;
}