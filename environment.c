#include "environment.h"

Environment create_environment() {
    Environment env;
    env.values = create_hashmap(10, hash_string, malloc, free, (void (*)(void *))free_object, compare_string, hashmap_copy_object, hashmap_copy_string, FLAG_COPY_VALUE);
    env.enclosing = NULL;
    return env;
}

Environment create_environment_from_enclosing(Environment enclosing) {
    Environment env = create_environment();
    env.enclosing = malloc(sizeof(Environment));
    *env.enclosing = enclosing;
    return env;
}

void delete_environment(Environment *env) {
    if(!env) return;
    free(env->enclosing);
    delete_hashmap(&env->values);
}

void environment_define(Environment env, const char *name, Object value) {
    hashmap_insert(&env.values, (void *)name, &value);
}

Object environment_get(Environment *env, Token name, bool *error) {
    if(hashmap_contains_key(&env->values, name.lexeme)) {
        return *(Object *)hashmap_get(&env->values, name.lexeme);
    }
    if(env->enclosing != NULL) return environment_get(env->enclosing, name, error);
    *error = true;
    return OBJECT_NONE();
}

void environment_assign(Environment *env, Token name, Object value, bool *error) {
    if(hashmap_contains_key(&env->values, name.lexeme)) {
        hashmap_insert(&env->values, name.lexeme, &value);
        return;
    }
    if(env->enclosing != NULL) {
        environment_assign(env->enclosing, name, value, error);
        return;
    }
    *error = true;
}
