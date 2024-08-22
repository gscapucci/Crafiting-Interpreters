#ifndef HASMAP_H
#define HASMAP_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct HashMap HashMap;
typedef struct HashMapItem HashMapItem;
enum Flag {
    FLAG_COPY_VALUE,
    FLAG_MOVE_VALUE,
};

struct HashMapItem {
    void *key;
    void *value;
    HashMapItem *next;
};


struct HashMap {
    HashMapItem **items;
    enum Flag flag;
    uint64_t cap;
    uint64_t size;
    uint64_t (*hash)(void *key);
    void * (*malloc)(uint64_t size);
    void (*free_key)(void *ptr);
    void (*free_value)(void *ptr);
    int64_t (*compare)(void *key1, void *key2);
    void (*copy_value)(void **dst, void *src);
    void (*copy_key)(void **dst, void *src);
};
//Set 'copy_value' to NULL if FLAG_MOVE_VALUE is setted
//Inside the 'copy_value' function check if dst is NULL
//If is NULL then alloc the memory before copy
//'delete_hashmap' function will free this memory
//In function 'copy_key', 'dst' will always be NULL, you need to alloc memory for 'dst'
HashMap create_hashmap( uint64_t cap,
                        uint64_t (*hash_function)(void *data),
                        void* (*your_alloc)(uint64_t size),
                        void (*your_free_key)(void *ptr),
                        void (*your_free_value)(void *ptr),
                        int64_t (*compare)(void *key1, void *key2),
                        void (*copy_value)(void **dst, void *src),
                        void (*copy_key)(void **dst, void *src),
                        enum Flag flag);
void delete_hashmap(HashMap *hashmap);
void hashmap_remove(HashMap *hashmap, void *key);
void hashmap_insert(HashMap *hashmap, void *key, void *value);
void hashmap_resize(HashMap *hashmap);
void *hashmap_get(HashMap *hashmap, void *key);
HashMapItem* new_hashmap_item(HashMap *hashmap, void *key, void *value);
bool hashmap_contains_key(HashMap *hashmap, void *key);


int64_t compare_string(void *str1, void *str2);
uint64_t hash_string(void *data);

void hashmap_copy_string(void **dst, void *src);
void hashmap_copy_object(void **dst, void *src);

#endif /* HASMAP_H */