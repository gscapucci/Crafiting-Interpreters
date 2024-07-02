#include "../include/expr.h"

Expr visit(Expr *expr) {
    switch (expr->type)
    {
    case EXPR_BINARY:
        return visit_binary_expr(expr);
        break;
    case EXPR_GROUPING:
        return visit_binary_grouping(expr);
        break;
    case EXPR_LITERAL:
        return visit_binary_literal(expr);
        break;
    case EXPR_UNARY:
        return visit_binary_unary(expr);
        break;
    default:
        error(0, "Unreachable");
        break;
    }
}


Expr visit_binary_expr(Expr *expr) {
    
}
Expr visit_binary_grouping(Expr *expr);
Expr visit_binary_literal(Expr *expr);
Expr visit_binary_unary(Expr *expr);