#include "interpreter.h"
#include "macro.h"
#include "error.h"
#include <setjmp.h>

static Lox *lox = NULL;
typedef struct RuntimeError RuntimeError;
struct RuntimeError {
    jmp_buf buff;
    Token operator;
    char *msg;
} runtime_error_obj;


void throw_runtime_error(Token operator, const char *msg) {
    runtime_error_obj.msg = calloc(strlen(msg) + 1, 1);
    strcpy(runtime_error_obj.msg, msg);
    runtime_error_obj.operator = operator;
    longjmp(runtime_error_obj.buff, 1);
    exit(1); // unreachable
}

Interpreter create_interpreter(Lox *l) {
    Interpreter i;
    lox = l;
    return i;
}

void interpret(Interpreter *interpreter, Expr expr) {
    if(setjmp(runtime_error_obj.buff) == 1) {
        runtime_error(lox, runtime_error_obj.operator.line, runtime_error_obj.msg);
        free(runtime_error_obj.msg);
        return;
    }

    Object value = evaluate(interpreter, expr);
    char *text = stringfy(value);
    if(text != NULL) {
        printf("%s", text);
        free(text);
    }
    delete_object(&value);
}

Object interpreter_accept(Interpreter *interpreter, Expr expr) {
    switch (expr.type)
    {
    case ExprTypeBinary:
        return interpreter_visit_binary_expr(interpreter, expr.binary);
    case ExprTypeGrouping:
        return interpreter_visit_grouping_expr(interpreter, expr.grouping);
    case ExprTypeLiteral:
        return interpreter_visit_literal_expr(interpreter, expr.literal);
    case ExprTypeUnary:
        return interpreter_visit_unary_expr(interpreter, expr.unary);
    }
    fprintf(stderr, "Unreachable");
    fflush(stdout);
    exit(1);
}

Object interpreter_visit_literal_expr(Interpreter *interpreter, ExprLiteral expr) {
    return expr.value;
}
Object interpreter_visit_grouping_expr(Interpreter *interpreter, ExprGrouping expr) {
    return evaluate(interpreter, *expr.expression);
}
Object interpreter_visit_unary_expr(Interpreter *interpreter, ExprUnary expr) {
    Object right = evaluate(interpreter, *expr.right);
    switch(expr.operator.type) {
        case MINUS:
            check_number_operand(expr.operator, right);
            switch(right.object_type) {
                case OBJ_TYPE_INT:
                    right.value.i_num *= -1;
                    return right;
                case OBJ_TYPE_FLOAT:
                    right.value.f_num *= -1.0;
                    return right;
                default:
                    throw_runtime_error(expr.operator, "Unreachable");
                    break;
            }
            break;
        case BANG:
            Object obj = create_object_from_bool(!is_truthy(right));
            if(obj.object_type == OBJ_TYPE_STRING) {
                free(obj.value.str);
            }
            return obj;
        default:
            throw_runtime_error(expr.operator, "Unreachable");
            break;
    }
    throw_runtime_error(expr.operator, "Unreachable");
    return OBJECT_NONE();
}

Object interpreter_visit_binary_expr(Interpreter *interpreter, ExprBinary expr) {
    Object left = evaluate(interpreter, *expr.left);
    Object right = evaluate(interpreter, *expr.right);

    switch(expr.operator.type) {
        case GREATER:
            check_comparison_operator(expr.operator, left, right);
            return create_object_from_bool(object_compare(left, right) > 0);
        case GREATER_EQUAL:
            check_comparison_operator(expr.operator, left, right);
            return create_object_from_bool(object_compare(left, right) >= 0);
        case LESS:
            check_comparison_operator(expr.operator, left, right);
            return create_object_from_bool(object_compare(left, right) < 0);
        case LESS_EQUAL:
            check_comparison_operator(expr.operator, left, right);
            return create_object_from_bool(object_compare(left, right) <= 0);
        case MINUS:
            check_number_operands(expr.operator, left, right);
            return object_minus(left, right);
        case SLASH:
            check_number_operands(expr.operator, left, right);
            return object_divide(left, right);
        case STAR:
            check_number_operands(expr.operator, left, right);
            return object_multiply(left, right);
        case PLUS:
            if(left.object_type == OBJ_TYPE_INT && right.object_type == OBJ_TYPE_INT) {
                left.value.i_num += right.value.i_num;
                return left;
            }
            if(left.object_type == OBJ_TYPE_FLOAT && right.object_type == OBJ_TYPE_FLOAT) {
                left.value.f_num += right.value.f_num;
                return left;
            }
            if(left.object_type == OBJ_TYPE_STRING && right.object_type == OBJ_TYPE_STRING) {
                uint64_t len = strlen(left.value.str) + strlen(right.value.str);
                char *str = calloc(len + 1, 1);
                strcat(str, left.value.str);
                strcat(str, right.value.str);
                Object obj = (Object) {
                    .object_type = OBJ_TYPE_STRING,
                    .value.str = str
                };
                free(left.value.str);
                free(right.value.str);
                return obj;
            }
            throw_runtime_error(expr.operator, "Operands must be integers, floats or strings.");
            break;
        case BANG_EQUAL:
            if(left.object_type != right.object_type) throw_runtime_error(expr.operator, "Operators must be of the same type.");
            Object obj1 = create_object_from_bool(!is_equal(left, right));
            if(left.object_type == OBJ_TYPE_STRING) {
                free(left.value.str);
                free(right.value.str);
            }
            return obj1;
        case EQUAL_EQUAL:
            if(left.object_type != right.object_type) throw_runtime_error(expr.operator, "Operators must be of the same type.");
            Object obj2 = create_object_from_bool(is_equal(left, right));
            if(left.object_type == OBJ_TYPE_STRING) {
                free(left.value.str);
                free(right.value.str);
            }
            return obj2;
        default:
            break;
    }
    throw_runtime_error(expr.operator, "Unreachable");
    return OBJECT_NONE();
}

Object evaluate(Interpreter *interpreter, Expr expr) {
    return interpreter_accept(interpreter, expr);
}

bool is_truthy(Object obj) {
    switch(obj.object_type) {
        case OBJ_TYPE_BOOL:
            return obj.value.b;
        case OBJ_TYPE_FLOAT:
            return true;
        case OBJ_TYPE_INT:
            return obj.value.i_num != 0;
        case OBJ_TYPE_STRING:
            return strlen(obj.value.str) > 0;
        case OBJ_TYPE_NONE:
        case OBJ_TYPE_NULL:
            return false;
        default:
            fprintf(stderr, "Unreachable");
            fflush(stdout);
            exit(1);
    }
}

bool is_equal(Object left, Object right) {
    return object_compare(left, right) == 0;
}

void check_comparison_operator(Token operator, Object left, Object right) {
    if(left.object_type == OBJ_TYPE_FLOAT && right.object_type == OBJ_TYPE_FLOAT) return;
    if(left.object_type == OBJ_TYPE_INT && right.object_type == OBJ_TYPE_INT) return;
    if(left.object_type == OBJ_TYPE_STRING && right.object_type == OBJ_TYPE_STRING) return;
    throw_runtime_error(operator, "Comparison operator error.");
}

void check_number_operand(Token operator, Object operand) {
    if(operand.object_type == OBJ_TYPE_INT || operand.object_type == OBJ_TYPE_FLOAT) return;
    throw_runtime_error(operator, "Operand must be a number");
}
void check_number_operands(Token operator, Object left, Object right) {
    if(left.object_type == OBJ_TYPE_FLOAT && right.object_type == OBJ_TYPE_FLOAT) return;
    if(left.object_type == OBJ_TYPE_INT && right.object_type == OBJ_TYPE_INT) return;
    throw_runtime_error(operator, "Both Operands must be integers or floats.");
}

char *stringfy(Object obj) {
    switch(obj.object_type) {
        case OBJ_TYPE_BOOL:
            return obj.value.b ? 
                        memcpy(malloc(sizeof("true")), "true", sizeof("true")) :
                        memcpy(malloc(sizeof("false")), "false", sizeof("false"));
        case OBJ_TYPE_NONE:
            return memcpy(malloc(sizeof("NONE")), "NONE", sizeof("NONE")); 
        case OBJ_TYPE_NULL:
            return memcpy(malloc(sizeof("null")), "null", sizeof("null"));
        case OBJ_TYPE_FLOAT:
            {
                char *text = calloc(20, 1);
                sprintf(text, "%f", obj.value.f_num);
                for(int64_t i = strlen(text);; i--) {
                    if(i <= 0) break;
                    if(text[i] == '0') text[i] = 0;
                    if(text[i] == '.') {
                        text[i + 1] = '0';
                        break;
                    }
                }
                return text;
            }
        case OBJ_TYPE_INT:
            {
                char *text = calloc(20, 1);
                sprintf(text, "%ld", obj.value.i_num);
                return text;
            }
        case OBJ_TYPE_STRING:
            {
                char *text = malloc(strlen(obj.value.str) + 1);
                return memcpy(text, obj.value.str, strlen(obj.value.str) + 1);
            }
    }
    return NULL;
}