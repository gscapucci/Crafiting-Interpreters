#ifndef EXPR_H
#define EXPR_H

#include "token.h"

typedef struct Expr Expr;
typedef struct ExprBinary ExprBinary;
typedef struct ExprGrouping ExprGrouping;
typedef struct ExprLiteral ExprLiteral;
typedef struct ExprUnary ExprUnary;
typedef struct ExprVariable ExprVariable;

enum ExprType {
    ExprTypeNULL = 0,
    ExprTypeBinary,
    ExprTypeGrouping,
    ExprTypeLiteral,
    ExprTypeUnary,
    ExprTypeVariable
};

struct ExprBinary {
    Expr *left;
    Token operator;
    Expr *right;
};

struct ExprGrouping {
    Expr *expression;
};

struct ExprLiteral {
    Object value;
};

struct ExprUnary {
    Token operator;
    Expr *right;
};

struct ExprVariable {
    Token name;
};


struct Expr {
    enum ExprType type;
    union {
        ExprBinary binary;
        ExprGrouping grouping;
        ExprLiteral literal;
        ExprUnary unary;
        ExprVariable variable;
    };
};

// The values insided the argumnets are moved, not copied
Expr create_binary_expr(Expr left, Token operator, Expr right);
// The values insided the argumnets are moved, not copied
Expr create_grouping_expr(Expr expression);
// The values insided the argumnets are moved, not copied
Expr create_literal_expr(Object value);
// The values insided the argumnets are moved, not copied
Expr create_unary_expr(Token operator, Expr right);
// The values insided the argumnets are moved, not copied
Expr create_variable_expr(Token name);
void delete_expr_tree(Expr *expr);
void free_expr_tree(Expr *expr);

#endif /* EXPR_H */