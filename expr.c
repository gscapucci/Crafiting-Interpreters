#include "expr.h"


Expr create_binary_expr(Expr left, Token operator, Expr right) {
    Expr expr;
    expr.type = ExprTypeBinary;
    
    expr.binary.left = malloc(sizeof(Expr));
    *expr.binary.left = left;
    
    expr.binary.right = malloc(sizeof(Expr));
    *expr.binary.right = right;

    expr.binary.operator = operator;
    
    return expr;
}


Expr create_grouping_expr(Expr expression) {
    Expr expr;
    expr.type = ExprTypeGrouping;
    expr.grouping.expression = malloc(sizeof(Expr));
    *expr.grouping.expression = expression;
    return expr;
}

Expr create_literal_expr(Object value) {
    Expr expr;
    expr.type = ExprTypeLiteral;
    expr.literal.value = value;
    return expr;
}

Expr create_unary_expr(Token operator, Expr right) {
    Expr expr;
    expr.type = ExprTypeUnary;
    expr.unary.operator = operator;
    expr.unary.right = malloc(sizeof(Expr));
    *expr.unary.right = right;
    return expr;
}