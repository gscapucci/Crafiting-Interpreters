#include "stmt.h"


Stmt create_stmt_print(Expr value) {
    Stmt stmt;
    stmt.type = StmtTypePrint;
    stmt.print.expr = value;
    return stmt;
}

Stmt create_stmt_expression(Expr expr) {
    Stmt stmt;
    stmt.type = StmtTypeExpression;
    stmt.expression.expr = expr;
    return stmt;
}

Stmt create_stmt_var(Token name, Expr init) {
    Stmt stmt;
    stmt.type = StmtTypeVar;
    stmt.var.name = name;
    stmt.var.initializer = init;
    return stmt;
}

void delete_stmt(Stmt *stmt) {
    if(!stmt)return;
    switch(stmt->type) {
        case StmtTypeExpression:
            delete_expr_tree(&stmt->expression.expr);
            break;
        case StmtTypePrint:
            delete_expr_tree(&stmt->print.expr);
            break;
        case StmtTypeVar:
            delete_expr_tree(&stmt->var.initializer);
            break;
        case StmtNULL:
            break;
    }
}