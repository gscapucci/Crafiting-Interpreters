#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "expr.h"

char *print(Expr expr);
char *ast_accept(Expr expr);
char *ast_visit_binary_expr(ExprBinary expr);
char *ast_visit_grouping_expr(ExprGrouping expr);
char *ast_visit_literal_expr(ExprLiteral expr);
char *ast_visit_unary_expr(ExprUnary expr);
char *parenthesize(char *name, uint64_t n_exprs, Expr *exprs);
#endif /* AST_PRINTER_H */