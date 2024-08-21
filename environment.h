#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "hashmap.h"
#include "object.h"
#include "token.h"
typedef struct Environment Environment;

struct Environment {
    HashMap values;
};
Environment create_environment();
void delete_environment(Environment *env);
void environment_define(Environment env, const char *name, Object value);
Object environment_get(Environment *env, Token name);
void environment_assign(Environment *env, Token name, Object value);

#endif /* ENVIRONMENT_H */