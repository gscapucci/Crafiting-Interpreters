#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "expr.h"

char *print(Expr expr);
char *ast_accept(Expr expr);
char *visit_binary_expr(ExprBinary expr);
char *visit_grouping_expr(ExprGrouping expr);
char *visit_literal_expr(ExprLiteral expr);
char *visit_unary_expr(ExprUnary expr);
char *parenthesize(char *name, uint64_t n_exprs, Expr *exprs);
#endif /* AST_PRINTER_H */