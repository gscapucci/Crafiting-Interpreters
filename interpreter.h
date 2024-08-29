#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "lox.h"
#include "stmt.h"
#include "environment.h"

typedef struct Interpreter Interpreter;

struct Interpreter {
    Environment env;
};

Interpreter create_interpreter(Lox *lox);
void delete_interpreter(Interpreter *interpreter);
void interpret(Interpreter *interpreter, StmtVec stmts);

Object interpreter_accept_expr(Interpreter *interpreter, Expr expr);
Object interpreter_visit_literal_expr(ExprLiteral expr);
Object interpreter_visit_grouping_expr(Interpreter *interpreter, ExprGrouping expr);
Object interpreter_visit_unary_expr(Interpreter *interpreter, ExprUnary expr);
Object interpreter_visit_binary_expr(Interpreter *interpreter, ExprBinary expr);
Object interpreter_visit_variable_expr(Interpreter *interpreter, ExprVariable expr);
Object interpreter_visit_assign_expr(Interpreter *interpreter, ExprAssign expr);
Object interpreter_visit_logical_expr(Interpreter *interpreter, ExprLogical expr);

Object evaluate(Interpreter *interpreter, Expr expr);

void interpreter_accept_stmt(Interpreter *interpreter, Stmt stmt);
void interpreter_visit_expression_stmt(Interpreter *interpreter, StmtExpression stmt);
void interpreter_visit_print_stmt(Interpreter *interpreter, StmtPrint stmt);
void interpreter_visit_var_stmt(Interpreter *interpreter, StmtVar stmt);
void interpreter_visit_block_stmt(Interpreter *interpreter, StmtBlock stmt);
void interpreter_visit_if_stmt(Interpreter *interpreter, StmtIf stmt);

void execute_block(Interpreter *interpreter, StmtVec statements, Environment env);
void execute(Interpreter *interpreter, Stmt stmt);

bool is_truthy(Object obj);
bool is_equal(Object left, Object right);
void check_comparison_operator(Token operator, Object left, Object right);
void check_number_operand(Token operator, Object operand);
void check_number_operands(Token operator, Object left, Object right);
char *stringfy(Object obj);
#endif /* INTERPRETER_H */