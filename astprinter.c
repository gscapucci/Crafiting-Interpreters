#include "astprinter.h"

char *print(Expr expr) {
    return ast_accept(expr);
}

char *ast_accept(Expr expr) {
    switch(expr.type) {
        case ExprTypeBinary:
            return ast_visit_binary_expr(expr.binary);
        case ExprTypeGrouping:
            return ast_visit_grouping_expr(expr.grouping);
        case ExprTypeLiteral:
            return ast_visit_literal_expr(expr.literal);
        case ExprTypeUnary:
            return ast_visit_unary_expr(expr.unary);
    }
    return NULL;
}


char *ast_visit_binary_expr(ExprBinary expr) {
    Expr exprs[] = {*expr.left, *expr.right};
    return parenthesize(expr.operator.lexeme, 2, exprs);
}
char *ast_visit_grouping_expr(ExprGrouping expr) {
    return parenthesize("group", 1, expr.expression);
}
char *ast_visit_literal_expr(ExprLiteral expr) {
    if(expr.value.object_type == OBJ_TYPE_NULL) return memcpy(malloc(4), "nil", 4);
    return object_to_string(&expr.value);
}
char *ast_visit_unary_expr(ExprUnary expr) {
    return parenthesize(expr.operator.lexeme, 1, expr.right);
}

void str_append(char **str1, char *str2) {
    uint64_t len1 = strlen(*str1);
    uint64_t len2 = strlen(str2);

    char *new_str = realloc(*str1, len1 + len2 + 1);
    if (new_str == NULL) {
        return;
    }

    memcpy(new_str + len1, str2, len2);
    new_str[len1 + len2] = '\0';
    *str1 = new_str;
}
char *parenthesize(char *name, uint64_t n_exprs, Expr *exprs) {
    char *res = memcpy(malloc(sizeof(char)), &(char){'\0'}, 1);
    str_append(&res, "(");
    str_append(&res, name);

    for(uint64_t i = 0; i < n_exprs; i++) {
        str_append(&res, " ");
        char *rep = ast_accept(exprs[i]);
        str_append(&res, rep);
        free(rep);
    }
    str_append(&res, ")");
    return res;
}