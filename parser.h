#ifndef PARSER_H
#define PARSER_H

#include "token_vec.h"
#include "token.h"
#include "expr.h"
#include "lox.h"

typedef struct Parser Parser;
//Parser don't need delete function, the Parser don't own any memory
struct Parser {
    TokenVec vec;
    int64_t current;
};

// move the values inside vec
Parser create_parser(Lox *lox, TokenVec vec);
Expr *parse(Parser *parser);
Expr *expression(Parser *parser);
Expr *equality(Parser *parser);
bool parser_match(Parser *parser, uint64_t count, const enum TokenType *types);
bool check(Parser *parser, enum TokenType type);
Token parser_advance(Parser *parser);
bool parser_is_at_end(Parser *parser);
Token parser_peek(Parser *parser);
Token previous(Parser *parser);
Expr *comparison(Parser *parser);
Expr *term(Parser *parser);
Expr *factor(Parser *parser);
Expr *unary(Parser *parser);
Expr *primary(Parser *parser);
Token consume(Parser *parser, enum TokenType type, const char *message);
void free_expr_tree(Expr *expr);


#endif /* PARSER_H */