#ifndef PARSER_H
#define PARSER_H

#include "token_vec.h"
#include "token.h"
#include "expr.h"
#include "lox.h"
#include "stmt.h"

typedef struct Parser Parser;
//Parser don't need delete function, the Parser don't own any memory
struct Parser {
    TokenVec vec;
    int64_t current;
    bool had_error;
};

// move the values inside vec
Parser create_parser(Lox *lox, TokenVec vec);
StmtVec parse(Parser *parser);
Stmt statement(Parser *parser);
Stmt declaration(Parser *parser);
Stmt print_statement(Parser *parser);
Stmt println_statement(Parser *parser);
Stmt expression_statement(Parser *parser);
Stmt var_declaration(Parser *parser);
Stmt if_statement(Parser *parser);
StmtVec block(Parser *parser);
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
Expr *assignment(Parser *parser);
Expr *or(Parser *parser);
Expr *and(Parser *parser);
Token consume(Parser *parser, enum TokenType type, const char *message);
void synchronize(Parser *parser);

#endif /* PARSER_H */