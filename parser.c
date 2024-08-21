#include "parser.h"
#include "error.h"
#include "macro.h"
#include <setjmp.h>

static Lox *lox = NULL;
typedef struct ParseError ParseError;
struct ParseError {
    jmp_buf buff;
    char *msg;
} parse_error_obj;


void throw_parse_error(Token token, const char *message);

Parser create_parser(Lox *l, TokenVec vec) {
    Parser parser;
    lox = l;
    parser.current = 0;
    parser.vec = vec;
    return parser;
}

StmtVec parse(Parser *parser) {
    StmtVec statements = create_stmt_vec();
    while(!parser_is_at_end(parser)) {
        stmt_vec_push(&statements, declaration(parser));
    }
    return statements;
}

Stmt statement(Parser *parser) {
    if(parser_match(parser, 1, &(enum TokenType){PRINT})) return print_statement(parser);
    return expression_statement(parser);
}

Stmt declaration(Parser *parser) {
    if(setjmp(parse_error_obj.buff) == 1) {
        // fprintf(stderr, "%s\n", parse_error_obj.msg);
        free(parse_error_obj.msg);
        synchronize(parser);
        return (Stmt){0};
    }
    if(parser_match(parser, 1, &(enum TokenType){VAR})) {
        return var_declaration(parser);
    }
    return statement(parser);
}

Stmt expression_statement(Parser *parser) {
    Expr *expr = expression(parser);
    consume(parser, SEMICOLON, "Exprect ';' after expression.");
    Stmt stmt = create_stmt_expression(*expr);
    free(expr);
    return stmt;
}

Stmt print_statement(Parser *parser) {
    Expr *value = expression(parser);
    consume(parser, SEMICOLON, "Expect ';' after value.");
    Stmt stmt = create_stmt_print(*value);
    free(value);
    return stmt;
}

Stmt var_declaration(Parser *parser) {
    Token name = consume(parser, IDENTIFIER, "Expect variable name.");
    Expr *initializer = NULL;
    if(parser_match(parser, 1, &(enum TokenType){EQUAL})) {
        initializer = expression(parser);
    }
    consume(parser, SEMICOLON, "Expect ';' after variable declaration.");
    Stmt stmt = create_stmt_var(name, *initializer);
    if(initializer != NULL) free(initializer);
    return stmt;
}

Expr *expression(Parser *parser) {
    return assignment(parser);
}

Expr *equality(Parser *parser) {
    Expr *expr = comparison(parser);
    enum TokenType aux[] = {BANG_EQUAL, EQUAL_EQUAL};
    while(parser_match(parser, 2, aux)) {
        Token operator = previous(parser);
        Expr *right = comparison(parser);
        *expr = create_binary_expr(*expr, operator, *right);
        free(right);
    }
    return expr;
}

Expr *comparison(Parser *parser) {
    Expr *expr = term(parser);
    enum TokenType aux[] = {GREATER, GREATER_EQUAL, LESS, LESS_EQUAL};
    while(parser_match(parser, 4, aux)) {
        Token operator = previous(parser);
        Expr *right = term(parser);
        *expr = create_binary_expr(*expr, operator, *right);
        free(right);
    }
    return expr;
}

Expr *term(Parser *parser) {
    Expr *expr = factor(parser);
    enum TokenType aux[] = {MINUS, PLUS};
    while(parser_match(parser, 2, aux)) {
        Token operator = previous(parser);
        Expr *right = factor(parser);
        *expr = create_binary_expr(*expr, operator, *right);
        free(right);
    }
    return expr;
}

Expr *factor(Parser *parser) {
    Expr *expr = unary(parser);
    enum TokenType aux[] = {SLASH, STAR};
    while (parser_match(parser, 2, aux)) {
        Token operator = previous(parser);
        Expr *right = unary(parser);
        *expr = create_binary_expr(*expr, operator, *right);
        free(right);
    }
    return expr;
}

Expr *unary(Parser *parser) {
    enum TokenType aux[] = {BANG, MINUS};
    if(parser_match(parser, 2, aux)) {
        Token operator = previous(parser);
        Expr *right = unary(parser);
        Expr ret = create_unary_expr(operator, *right);
        return memcpy(malloc(sizeof(Expr)), &ret, sizeof(Expr));
    }
    return primary(parser);
}

Expr *primary(Parser *parser) {
    if(parser_match(parser, 1, &(enum TokenType){FALSE})) {
        Expr expr = create_literal_expr((Object){.object_type = OBJ_TYPE_BOOL, .value.b = false});
        return memcpy(malloc(sizeof(Expr)), &expr, sizeof(Expr));
    }
    if(parser_match(parser, 1, &(enum TokenType){TRUE})) {
        Expr expr = create_literal_expr((Object){.object_type = OBJ_TYPE_BOOL, .value.b = true});
        return memcpy(malloc(sizeof(Expr)), &expr, sizeof(Expr));
    }
    if(parser_match(parser, 1, &(enum TokenType){NIL})) {
        Expr expr = create_literal_expr((Object){.object_type = OBJ_TYPE_NULL, .value = {0}});
        return memcpy(malloc(sizeof(Expr)), &expr, sizeof(Expr));
    }
    enum TokenType aux[] = {FLOAT, INT, STRING};
    if(parser_match(parser, 3, aux)) {
        Expr expr = create_literal_expr(previous(parser).literal);
        return memcpy(malloc(sizeof(Expr)), &expr, sizeof(Expr));
    }
    if(parser_match(parser, 1, &(enum TokenType){IDENTIFIER})) {
        Expr expr = create_variable_expr(previous(parser));
        return memcpy(malloc(sizeof(Expr)), &expr, sizeof(expr));
    }
    if(parser_match(parser, 1, &(enum TokenType){LEFT_PAREN})) {
        Expr *expr = expression(parser);
        consume(parser, RIGHT_PAREN, "Expect ')', after expression.");
        Expr group = create_grouping_expr(*expr);
        free(expr);
        return memcpy(malloc(sizeof(Expr)), &group, sizeof(Expr));
    }
    throw_parse_error(parser_peek(parser), "Unreachable");
    return NULL;
}

Expr *assignment(Parser *parser) {
    Expr *expr = equality(parser);
    if(parser_match(parser, 1, &(enum TokenType){EQUAL})) {
        Token equals = previous(parser);
        Expr *value = assignment(parser);
        if(expr->type == ExprTypeVariable) {
            Token name = expr->variable.name;
            Expr aux = create_assign_expr(name, *value);
            free(expr);
            free(value);
            Expr *ret = memcpy(malloc(sizeof(Expr)), &aux, sizeof(Expr));
            return ret;
        }
        error_token(lox, equals, "Invalid assignment target.");
    }
    return expr;
}

Token consume(Parser *parser, enum TokenType type, const char *message) {
    if(check(parser, type)) return parser_advance(parser);
    throw_parse_error(parser_peek(parser), message);
    exit(1); //unreachable
}

void throw_parse_error(Token token, const char *message) {
    parse_error_obj.msg = error_token_as_str(lox, token, message);
    longjmp(parse_error_obj.buff, 1);
}

bool parser_match(Parser *parser, uint64_t count, const enum TokenType *types) {
    for(uint64_t i = 0; i < count; i++) {
        if(check(parser, types[i])) {
            parser_advance(parser);
            return true;
        }
    }
    return false;
}

bool check(Parser *parser, enum TokenType type) {
    if(parser_is_at_end(parser)) return false;
    return parser_peek(parser).type == type;
}

Token parser_advance(Parser *parser) {
    if(!parser_is_at_end(parser)) parser->current++;
    return previous(parser);
}

bool parser_is_at_end(Parser *parser) {
    return parser_peek(parser).type == EEOF;
}

Token parser_peek(Parser *parser) {
    return parser->vec.tokens[parser->current];
}

Token previous(Parser *parser) {
    return parser->vec.tokens[parser->current - 1];
}

void synchronize(Parser *parser) {
    parser_advance(parser);
    while(!parser_is_at_end(parser)) {
        if(previous(parser).type == SEMICOLON) return;
        switch(parser_peek(parser).type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
            default:
                break;
        }
        parser_advance(parser);
    }
}