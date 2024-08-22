#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "hashmap.h"
#include "object.h"
#include "token.h"
typedef struct Environment Environment;

struct Environment {
    HashMap values;
    Environment *enclosing;
};
Environment create_environment();
Environment create_environment_from_enclosing(Environment enclosing);
void delete_environment(Environment *env);
void environment_define(Environment env, const char *name, Object value);
Object environment_get(Environment *env, Token name, bool *error);
void environment_assign(Environment *env, Token name, Object value, bool *error);

#endif /* ENVIRONMENT_H */