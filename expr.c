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

Expr create_variable_expr(Token name) {
    Expr expr;
    expr.type = ExprTypeVariable;
    expr.variable.name = name;
    return expr;
}

Expr create_assign_expr(Token name, Expr value) {
    Expr expr;
    expr.type = ExprTypeAssign;
    expr.assign.name = name;
    expr.assign.value = malloc(sizeof(Expr));
    *expr.assign.value = value;
    return expr;
}

void delete_expr_tree(Expr *expr) {
    if(!expr) return;
    switch(expr->type) {
        case ExprTypeBinary:
            free_expr_tree(expr->binary.left);
            free_expr_tree(expr->binary.right);
            break;
        case ExprTypeGrouping:
            free_expr_tree(expr->grouping.expression);
            break;
        case ExprTypeLiteral:
            break;
        case ExprTypeUnary:
            free_expr_tree(expr->unary.right);
            break;
        case ExprTypeAssign:
            free_expr_tree(expr->assign.value);
            break;
        case ExprTypeVariable:
            break;
        case ExprTypeNULL:
            break;
    }
}

void free_expr_tree(Expr *expr) {
    if(!expr) return;
    switch(expr->type) {
        case ExprTypeBinary:
            free_expr_tree(expr->binary.left);
            free_expr_tree(expr->binary.right);
            break;
        case ExprTypeGrouping:
            free_expr_tree(expr->grouping.expression);
            break;
        case ExprTypeLiteral:
            break;
        case ExprTypeUnary:
            free_expr_tree(expr->unary.right);
            break;
        case ExprTypeAssign:
            free_expr_tree(expr->assign.value);
            break;
        case ExprTypeVariable:
            break;
        case ExprTypeNULL:
            break;
    }
    free(expr);
    return;
}