#include "stmt_vec.h"


StmtVec create_stmt_vec() {
    StmtVec vec;
    vec.cap = 100;
    vec.stmts = malloc(sizeof(Stmt) * 100);
    vec.size = 0;
    return vec;
}

void delete_stmt_vec(StmtVec *stmt_vec) {
    if(!stmt_vec) return;
    if(!stmt_vec->stmts) return;
    for(uint64_t i = 0; i < stmt_vec->size; i++) {
        delete_stmt(&stmt_vec->stmts[i]);
    }
    free(stmt_vec->stmts);
}

void stmt_vec_push(StmtVec *vec, Stmt stmt) {
    if(!vec) return;
    if(vec->size >= vec->cap) {
        vec->cap *= 2;
        vec->stmts = realloc(vec->stmts, vec->cap);
    }
    vec->stmts[vec->size++] = stmt;
}