#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "lox.h"
typedef struct Interpreter Interpreter;

struct Interpreter {};

Interpreter create_interpreter(Lox *lox);
void interpret(Interpreter *interpreter, Expr expr);

Object interpreter_accept(Interpreter *interpreter, Expr expr);
Object interpreter_visit_literal_expr(Interpreter *interpreter, ExprLiteral expr);
Object interpreter_visit_grouping_expr(Interpreter *interpreter, ExprGrouping expr);
Object interpreter_visit_unary_expr(Interpreter *interpreter, ExprUnary expr);
Object interpreter_visit_binary_expr(Interpreter *interpreter, ExprBinary expr);
Object evaluate(Interpreter *interpreter, Expr expr);
bool is_truthy(Object obj);
bool is_equal(Object left, Object right);
void check_comparison_operator(Token operator, Object left, Object right);
void check_number_operand(Token operator, Object operand);
void check_number_operands(Token operator, Object left, Object right);
char *stringfy(Object obj);
#endif /* INTERPRETER_H */