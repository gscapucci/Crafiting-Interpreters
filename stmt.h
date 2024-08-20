#ifndef STMT_H
#define STMT_H

#include "expr.h"

typedef struct Stmt Stmt;
typedef struct StmtPrint StmtPrint;
typedef struct StmtExpression StmtExpression;
typedef struct StmtVar StmtVar;

enum StmtType {
    StmtNULL = 0,
    StmtTypeExpression,
    StmtTypePrint,
    StmtTypeVar
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
struct Stmt {
    enum StmtType type;
    union {
        StmtPrint print;
        StmtExpression expression;
        StmtVar var;
    };
};

Stmt create_stmt_print(Expr value);
Stmt create_stmt_expression(Expr expr);
Stmt create_stmt_var(Token name, Expr init);
void delete_stmt(Stmt *stmt);

#endif /* STMT_H */