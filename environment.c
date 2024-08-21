#include "environment.h"

Environment create_environment() {
    Environment env;
    env.values = create_hashmap(10, hash_string, malloc, free, compare_string, hashmap_copy_object, hashmap_copy_string, FLAG_COPY_VALUE);
    return env;
}

void delete_environment(Environment *env) {
    if(!env) return;
    delete_hashmap(&env->values);
}

void environment_define(Environment env, const char *name, Object value) {
    hashmap_insert(&env.values, (void *)name, &value);
}

Object environment_get(Environment *env, Token name) {
    if(hashmap_contains_key(&env->values, name.lexeme)) {
        return *(Object *)hashmap_get(&env->values, name.lexeme);
    }
    // throw_runtime_error();
    fprintf(stderr, "Undefined variable '%s'.", name.lexeme);
    exit(1);
}

void environment_assign(Environment *env, Token name, Object value) {
    if(hashmap_contains_key(&env->values, name.lexeme)) {
        hashmap_insert(&env->values, name.lexeme, &value);
        return;
    }
    // throw_runtime_error();
    fprintf(stderr, "Undefined variable '%s'.", name.lexeme);
    exit(1);
}
