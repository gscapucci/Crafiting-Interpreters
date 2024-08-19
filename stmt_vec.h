#ifndef STMT_VEC_H
#define STMT_VEC_H

#include "stmt.h"

typedef struct StmtVec StmtVec;

struct StmtVec {
    Stmt *stmts;
    uint64_t size;
    uint64_t cap;
};


StmtVec create_stmt_vec();
void delete_stmt_vec(StmtVec *stmt_vec);

void stmt_vec_push(StmtVec *vec, Stmt stmt);

#endif /* STMT_VEC_H */