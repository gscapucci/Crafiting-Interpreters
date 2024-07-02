#ifndef HASHMAP_H
#define HASHMAP_H

#include "token_type.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct HashMap HashMap;
typedef struct Pair Pair;

#define HASHMAP_START_CAP 25
#define GROWTH_FACTOR 2

struct Pair {
    char *key;
    enum TokenType type;
};

struct HashMap {
    Pair *data;
    uint64_t size;
    uint64_t cap;
};

uint32_t hash_string(const char *key);

HashMap create_hashmap();
void delete_hashmap(HashMap *map);
void hashmap_insert(HashMap *map, const char *key, enum TokenType type);
void hashmap_resize(HashMap *map);
enum TokenType hashmap_find(HashMap *map, const char *key);

#endif /* HASHMAP_H */