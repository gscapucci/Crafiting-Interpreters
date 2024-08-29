#ifndef STMT_H
#define STMT_H

#include "expr.h"

typedef struct StmtVec StmtVec;
typedef struct Stmt Stmt;
typedef struct StmtPrint StmtPrint;
typedef struct StmtExpression StmtExpression;
typedef struct StmtVar StmtVar;
typedef struct StmtBlock StmtBlock;
typedef struct StmtIf StmtIf;


struct StmtVec {
    Stmt *stmts;
    uint64_t size;
    uint64_t cap;
};


StmtVec create_stmt_vec();
StmtVec create_stmt_vec_with_start_cap(uint64_t cap);
void delete_stmt_vec(StmtVec *stmt_vec);

void stmt_vec_push(StmtVec *vec, Stmt stmt);


enum StmtType {
    StmtNULL = 0,
    StmtTypeExpression,
    StmtTypePrint,
    StmtTypeVar,
    StmtTypeBlock,
    StmtTypeIf
};

struct StmtPrint {
    Expr expr;
};
struct StmtExpression {
    Expr expr;
};
struct StmtVar {
    Token name;
    Expr initializer;
};
struct StmtBlock {
    StmtVec statements;
};
struct StmtIf {
    Expr condition;
    Stmt *thenBranch;
    Stmt *elseBranch;
};
struct Stmt {
    enum StmtType type;
    union {
        StmtPrint print;
        StmtExpression expression;
        StmtVar var;
        StmtBlock block;
        StmtIf iff;
    };
};

Stmt create_stmt_print(Expr value);
Stmt create_stmt_expression(Expr expr);
Stmt create_stmt_var(Token name, Expr init);
Stmt create_stmt_block(StmtVec vec);
void delete_stmt(Stmt *stmt);

#endif /* STMT_H */