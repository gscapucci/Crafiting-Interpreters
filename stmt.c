#include "stmt.h"

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

Stmt create_stmt_block(StmtVec stmts) {
    Stmt stmt;
    stmt.type = StmtTypeBlock;
    stmt.block.statements = stmts;
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
        case StmtTypeBlock:
            delete_stmt_vec(&stmt->block.statements);
            break;
        case StmtNULL:
            break;
    }
}
