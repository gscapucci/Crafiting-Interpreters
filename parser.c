#include "parser.h"
#include "error.h"
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

Expr *parse(Parser *parser) {
    if(setjmp(parse_error_obj.buff) == 1) {
        fprintf(stderr, "%s\n", parse_error_obj.msg);
        free(parse_error_obj.msg);
        return NULL;
    }

    return expression(parser);
}

Expr *expression(Parser *parser) {
    return equality(parser);
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
    if(parser_match(parser, 2, aux)) {
        Expr expr = create_literal_expr(previous(parser).literal);
        return memcpy(malloc(sizeof(Expr)), &expr, sizeof(Expr));
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
        default:
            break;
    }
    free(expr);
    return;
}