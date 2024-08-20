#include "hashmap.h"
#include "macro.h"
#include "object.h"

HashMap create_hashmap( uint64_t cap,
                        uint64_t (*hash_function)(void *data),
                        void* (*your_alloc)(uint64_t size),
                        void (*your_free)(void *ptr),
                        int64_t (*compare)(void *key1, void *key2),
                        void (*copy_value)(void **dst, void *src),
                        void (*copy_key)(void **dst, void *src),
                        enum Flag flag) {
    (void)flag;
    HashMap hashmap;
    hashmap.copy_value = flag == FLAG_COPY_VALUE ? copy_value : NULL;
    hashmap.copy_key = copy_key;
    hashmap.cap = cap;
    hashmap.size = 0;
    hashmap.hash = hash_function;
    hashmap.malloc = your_alloc;
    hashmap.free = your_free;
    hashmap.compare = compare;
    hashmap.flag = flag;
    hashmap.items = calloc(cap, sizeof(HashMapItem *));
    return hashmap;
}

void delete_hashmap(HashMap *hashmap) {
    if(!hashmap) return;

    for(uint64_t i = 0; i < hashmap->cap; i++) {
        HashMapItem *prev = NULL;
        HashMapItem *curr = hashmap->items[i];
        while(curr) {
            prev = curr;
            curr = curr->next;
            hashmap->free(prev->key);
            hashmap->free(prev->value);
            hashmap->free(prev);
        }
    }
    hashmap->free(hashmap->items);
}

void hashmap_remove(HashMap *mp, void *key) {
    uint64_t index = mp->hash(key) % mp->cap;
 
    HashMapItem *prev = NULL;
 
    // Points to the head of
    // linked list present at
    // bucket index
    HashMapItem *curr = mp->items[index];
 
    while (curr != NULL) {
 
        // Key is matched at delete this
        // node from linked list
        if (mp->compare(key, curr->key) == 0) {
 
            // Head node
            // deletion
            if (curr == mp->items[index]) {
                mp->items[index] = curr->next;
            }
 
            // Last node or middle node
            else {
                prev->next = curr->next;
            }
            mp->free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
}


void hashmap_insert(HashMap *hashmap, void *key, void *value) {
    uint64_t index = hashmap->hash(key) % hashmap->cap;
    HashMapItem *prev = NULL;
    HashMapItem *curr = NULL;
    for(curr = hashmap->items[index]; curr; prev = curr, curr = curr->next) {
        if(hashmap->compare(key, curr->key) == 0) {
            switch(hashmap->flag) {
                case FLAG_COPY_VALUE:
                    hashmap->copy_value(curr->value, value);
                    break;
                case FLAG_MOVE_VALUE:
                    hashmap->free(curr->value);
                    curr->value = value;
                    break;
            }
            return;
        }
    }
    if(prev == NULL) {
        hashmap->items[index] = new_hashmap_item(hashmap, key, value);
    } else {
        prev->next = new_hashmap_item(hashmap, key, value);
    }
}
void hashmap_resize(HashMap *hashmap) {
    (void)hashmap;
    TODO
}

void *hashmap_get(HashMap *hashmap, void *key) {
    if(!hashmap) return NULL;
    uint64_t index = hashmap->hash(key) % hashmap->cap;
    for(HashMapItem *curr = hashmap->items[index]; curr; curr = curr->next) {
        if(hashmap->compare(key, curr->key) == 0) {
            return curr->value;
        }
    }
    return NULL;
}

HashMapItem* new_hashmap_item(HashMap *hashmap, void *key, void *value) {
    HashMapItem *item = hashmap->malloc(sizeof(HashMapItem));
    memset(item, 0, sizeof(HashMapItem));
    switch(hashmap->flag) {
        case FLAG_COPY_VALUE:
            hashmap->copy_value(&item->value, value);
            break;
        case FLAG_MOVE_VALUE:
            item->value = value;
            break;
    }
    hashmap->copy_key(&item->key, key);
    return item;
}

bool hashmap_contains_key(HashMap *hashmap, void *key) {
    uint64_t index = hashmap->hash(key) % hashmap->cap;
    for(HashMapItem *curr = hashmap->items[index]; curr; curr = curr->next) {
        if(hashmap->compare(key, curr->key) == 0) {
            return true;
        }
    }
    return false;
}

int64_t compare_string(void *str1, void *str2) {
    return strcmp(str1, str2);
}

uint64_t hash_string(void *data) {
    uint64_t len = strlen(data);
    uint64_t hash_value = 0;
    for(uint64_t i = 0; i < len; i++) {
        hash_value += ((char *)data)[i];
        hash_value = (hash_value * ((char *)data)[i]);
    }
    return hash_value;
}

void hashmap_copy_string(void **dst, void *src) {
    if(*dst == NULL) {
        *dst = calloc(strlen(src) + 1, 1);
    }
    strcpy(*dst, src);
}
void hashmap_copy_object(void **dst, void *src) {
    if(*dst == NULL) {
        *dst = calloc(sizeof(Object), 1);
    }
    **(Object **)dst = copy_object((Object *)src);
}