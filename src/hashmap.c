#include "../include/hashmap.h"

uint32_t hash_string(const char *key) {
    uint32_t hash = 2166136261u;
    for(uint64_t i = 0; i < strlen(key); i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}


HashMap create_hashmap() {
    HashMap map;
    map.data = (Pair *)calloc(HASHMAP_START_CAP, sizeof(Pair));
    map.size = 0;
    map.cap = HASHMAP_START_CAP;
    return map;
}

void delete_hashmap(HashMap *map) {
    if (!map || !map->data) return;
    for (uint64_t i = 0; i < map->cap; i++) {
        if (map->data[i].key) {
            free(map->data[i].key);
        }
    }
    free(map->data);
}

void hashmap_insert(HashMap *map, const char *key, enum TokenType type) {
    if (map->size >= map->cap / 2) {
        hashmap_resize(map);
    }
    
    uint32_t index = hash_string(key) % map->cap;
    while (map->data[index].key != NULL) {
        if (strcmp(map->data[index].key, key) == 0) {
            map->data[index].type = type;
            return;
        }
        index = (index + 1) % map->cap;
    }

    map->data[index].key = strdup(key);
    map->data[index].type = type;
    map->size++;
}

void hashmap_resize(HashMap *map) {
    uint64_t new_cap = map->cap * GROWTH_FACTOR;
    Pair *new_data = (Pair *)calloc(new_cap, sizeof(Pair));

    for (uint64_t i = 0; i < map->cap; i++) {
        if (map->data[i].key != NULL) {
            uint32_t new_index = hash_string(map->data[i].key) % new_cap;
            while (new_data[new_index].key != NULL) {
                new_index = (new_index + 1) % new_cap;
            }
            new_data[new_index] = map->data[i];
        }
    }

    free(map->data);
    map->data = new_data;
    map->cap = new_cap;
}

enum TokenType hashmap_find(HashMap *map, const char *key) {
    uint32_t index = hash_string(key) % map->cap;
    while (map->data[index].key != NULL) {
        if (strcmp(map->data[index].key, key) == 0) {
            return map->data[index].type;
        }
        index = (index + 1) % map->cap;
    }
    return -1; // Indica que a chave não foi encontrada
}