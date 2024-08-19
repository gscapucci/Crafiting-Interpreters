#ifndef STMT_H
#define STMT_H

#include "expr.h"

typedef struct Stmt Stmt;
typedef struct StmtPrint StmtPrint;
typedef struct StmtExpression StmtExpression;

enum StmtType {
    StmtTypeExpression,
    StmtTypePrint
};

struct StmtPrint {
    Expr expr;
};
struct StmtExpression {
    Expr expr;
};

struct Stmt {
    enum StmtType type;
    union {
        StmtPrint print;
        StmtExpression expression;
    };
};

Stmt create_stmt_print(Expr value);
Stmt create_stmt_expression(Expr expr);
void delete_stmt(Stmt *stmt);

#endif /* STMT_H */