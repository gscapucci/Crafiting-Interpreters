#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "lox.h"

typedef struct Expr Expr;

typedef struct Binary Binary;
typedef struct Grouping Grouping;
typedef struct Literal Literal;
typedef struct Unary Unary;

enum ExprType {
    EXPR_BINARY,
    EXPR_GROUPING,
    EXPR_LITERAL,
    EXPR_UNARY
};

struct Binary {
    Expr *left;
    Token operator;
    Expr *right;
};

struct Grouping {
    Expr *expression;
};

struct Literal {
    Object value;
};

struct Unary {
    Token operator;
    Expr *right;
};

struct Expr {
    enum ExprType type;
    union {
        Binary binary;
        Grouping grouping;
        Literal literal;
        Unary unary;
    };
};

Expr visit(Expr *expr);
Expr visit_binary_expr(Expr *expr);
Expr visit_binary_grouping(Expr *expr);
Expr visit_binary_literal(Expr *expr);
Expr visit_binary_unary(Expr *expr);


#endif /* EXPR_H */