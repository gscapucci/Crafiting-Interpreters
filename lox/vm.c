#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "assembly.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "vm.h"

VM vm;

static void reset_stack() {
    vm.stack_top = vm.stack;
}

static Value peek(int distance) {
    return vm.stack_top[-1 - distance];
}

static bool is_falsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void runtime_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = vm.chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    reset_stack();
}

static void concatenate(Value a, Value b) {
    ObjString *b1 = AS_STRING(b);
    ObjString *a1 = AS_STRING(a);

    int length = a1->length + b1->length;
    char *chars = ALLOCATE(char, length + 1);
    if (!chars) {
        runtime_error("Memory allocation failed.");
        return;
    }
    memcpy(chars, a1->chars, a1->length);
    memcpy(chars + a1->length, b1->chars, b1->length);
    chars[length] = '\0';

    ObjString *result = take_string(chars, length);
    push(OBJ_VAL(result));
}

void init_VM() {
    reset_stack();
    vm.objects = NULL;
    vm.options = OPT_NONE;
    init_table(&vm.strings);
}

void free_VM() {
    free_table(&vm.strings);
    free_objects();
}

static InterpretResult run() {
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) {
        #ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            for (Value *slot = vm.stack; slot < vm.stack_top; slot++) {
                printf("[ ");
                print_value(*slot);
                printf(" ]");
            }
            printf("\n");
            disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
        #endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_FALSE: push(BOOL_VAL(false)); break;
            case OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(values_equal(a, b)));
                break;
            }
            case OP_GREATER: {
                Value b = pop();
                Value a = pop();
                if (a.type != b.type) {
                    runtime_error("Operands must be of the same type for greater comparison.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (IS_FLOAT(a)) {
                    push(BOOL_VAL(AS_FLOAT(a) > AS_FLOAT(b)));
                } else if (IS_INT(a)) {
                    push(BOOL_VAL(AS_INT(a) > AS_INT(b)));
                } else if (IS_UINT(a)) {
                    push(BOOL_VAL(AS_UINT(a) > AS_UINT(b)));
                } else {
                    runtime_error("Operands must be numbers for greater comparison.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_LESS: {
                Value b = pop();
                Value a = pop();
                if (a.type != b.type) {
                    runtime_error("Operands must be of the same type for less comparison.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (IS_FLOAT(a)) {
                    push(BOOL_VAL(AS_FLOAT(a) < AS_FLOAT(b)));
                } else if (IS_INT(a)) {
                    push(BOOL_VAL(AS_INT(a) < AS_INT(b)));
                } else if (IS_UINT(a)) {
                    push(BOOL_VAL(AS_UINT(a) < AS_UINT(b)));
                } else {
                    runtime_error("Operands must be numbers for less comparison.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_ADD: {
                Value b = pop();
                Value a = pop();
                if (a.type != b.type) {
                    runtime_error("Operands must be of the same type for addition.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (IS_FLOAT(a)) {
                    push(FLOAT_VAL(AS_FLOAT(a) + AS_FLOAT(b)));
                } else if (IS_INT(a)) {
                    push(INT_VAL(AS_INT(a) + AS_INT(b)));
                } else if (IS_UINT(a)) {
                    push(UINT_VAL(AS_UINT(a) + AS_UINT(b)));
                } else if (IS_OBJ(a) && a.as.obj->type == OBJ_STRING) {
                    concatenate(a, b);
                } else {
                    runtime_error("Operands must be numbers or strings for addition.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SUBTRACT: {
                Value b = pop();
                Value a = pop();
                if (a.type != b.type) {
                    runtime_error("Operands must be of the same type for subtraction.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (IS_FLOAT(a)) {
                    push(FLOAT_VAL(AS_FLOAT(a) - AS_FLOAT(b)));
                } else if (IS_INT(a)) {
                    push(INT_VAL(AS_INT(a) - AS_INT(b)));
                } else if (IS_UINT(a)) {
                    push(UINT_VAL(AS_UINT(a) - AS_UINT(b)));
                } else {
                    runtime_error("Operands must be numbers for subtraction.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_MULTIPLY: {
                Value b = pop();
                Value a = pop();
                if (a.type != b.type) {
                    runtime_error("Operands must be of the same type for multiplication.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (IS_FLOAT(a)) {
                    push(FLOAT_VAL(AS_FLOAT(a) * AS_FLOAT(b)));
                } else if (IS_INT(a)) {
                    push(INT_VAL(AS_INT(a) * AS_INT(b)));
                } else if (IS_UINT(a)) {
                    push(UINT_VAL(AS_UINT(a) * AS_UINT(b)));
                } else {
                    runtime_error("Operands must be numbers for multiplication.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_DIVIDE: {
                Value b = pop();
                Value a = pop();
                if (a.type != b.type) {
                    runtime_error("Operands must be of the same type for division.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (IS_FLOAT(a)) {
                    if (AS_FLOAT(b) == 0.0) {
                        runtime_error("Division by zero.");
                        return INTERPRET_RUNTIME_ERROR;
                    }
                    push(FLOAT_VAL(AS_FLOAT(a) / AS_FLOAT(b)));
                } else if (IS_INT(a)) {
                    if (AS_INT(b) == 0) {
                        runtime_error("Division by zero.");
                        return INTERPRET_RUNTIME_ERROR;
                    }
                    push(INT_VAL(AS_INT(a) / AS_INT(b)));
                } else if (IS_UINT(a)) {
                    if (AS_UINT(b) == 0) {
                        runtime_error("Division by zero.");
                        return INTERPRET_RUNTIME_ERROR;
                    }
                    push(UINT_VAL(AS_UINT(a) / AS_UINT(b)));
                } else {
                    runtime_error("Operands must be numbers for division.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_NOT:
                push(BOOL_VAL(is_falsey(pop())));
                break;
            case OP_NEGATE: {
                Value a = pop();
                if (!IS_NUMBER(a)) {
                    runtime_error("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (IS_FLOAT(a)) {
                    a.as.float_number *= -1;
                } else if (IS_INT(a)) {
                    a.as.int_number *= -1;
                } else if (IS_UINT(a)) {
                    a.as.uint_number = (uint64_t)(-(int64_t)a.as.uint_number);
                }
                push(a);
                break;
            }
            case OP_RETURN: {
                print_value(pop());
                return INTERPRET_OK;
            }
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
}

InterpretResult interpret(const char *source) {
    Chunk chunk;
    init_chunk(&chunk);

    if (!compile(source, &chunk)) {
        free_chunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    free_chunk(&chunk);
    return result;
}

static CompileResult native_compile(const char *output_path) {
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    char *output_file_path = calloc(strlen(output_path) + 6, 1);
    if (!output_file_path) {
        fprintf(stderr, "Memory allocation failed.\n");
        return COMPILE_ERROR;
    }

    memcpy(output_file_path, output_path, strlen(output_path) - strlen(".lox"));
    strcat(output_file_path, ".fasm");

    FILE *output_file = fopen(output_file_path, "w");
    if (!output_file) {
        fprintf(stderr, "Error: Cannot open file for writing: %s\n", output_file_path);
        free(output_file_path);
        return COMPILE_ERROR;
    }

    ValueType *type_stack = ALLOCATE(ValueType, 200);
    if (!type_stack) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(output_file);
        free(output_file_path);
        return COMPILE_ERROR;
    }

    uint64_t type_stack_size = 0;
    uint64_t type_stack_cap = 200;
    CompileResult result = COMPILE_OK;

    uint64_t float_count = 0;
    double *floats = ALLOCATE(double, 200);
    if (!floats) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(output_file);
        free(output_file_path);
        return COMPILE_ERROR;
    }

    uint64_t floats_size = 0;
    uint64_t floats_cap = 200;

    // Escreve o cabeçalho do arquivo FASM
    write_asm_header(output_file);
    fprintf_asm_aux_functions(output_file);
    fprintf(output_file, "main:\n");

    for (;;) {
        if (type_stack_size >= type_stack_cap) {
            type_stack = GROW_ARRAY(ValueType, type_stack, type_stack_cap, type_stack_cap * 2);
        }
        if (floats_size >= floats_cap) {
            floats = GROW_ARRAY(double, floats, floats_cap, floats_cap * 2);
        }

        uint8_t instruction = READ_BYTE();
        switch (instruction) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                if(constant.type != VAL_UINT && constant.type != VAL_INT && constant.type != VAL_BOOL && constant.type != VAL_FLOAT) {
                    runtime_error("Unknown constant type in OP_CONSTANT.");
                }
                if(constant.type == VAL_FLOAT) {
                    floats[floats_size++] = constant.as.float_number;
                }
                write_constant(output_file, constant, &float_count);
                type_stack[type_stack_size++] = constant.type;
                break;
            }
            case OP_NIL:
                fprintf(output_file, "    ;; Push nil\n");
                fprintf(output_file, "    mov rax, 0\n");
                fprintf(output_file, "    push rax\n");
                type_stack[type_stack_size++] = VAL_NIL;
                break;
            case OP_TRUE:
                fprintf(output_file, "    ;; Push true\n");
                fprintf(output_file, "    mov rax, 1\n");
                fprintf(output_file, "    push rax\n");
                type_stack[type_stack_size++] = VAL_BOOL;
                break;
            case OP_FALSE:
                fprintf(output_file, "    ;; Push false\n");
                fprintf(output_file, "    mov rax, 0\n");
                fprintf(output_file, "    push rax\n");
                type_stack[type_stack_size++] = VAL_BOOL;
                break;
            case OP_EQUAL:
                if (type_stack_size < 2) {
                    runtime_error("Type stack underflow in comparison.");
                    result = COMPILE_ERROR;
                    break;
                }
                write_comparison(output_file, "je", type_stack, &type_stack_size);
                break;
            case OP_GREATER:
                if (type_stack_size < 2) {
                    runtime_error("Type stack underflow in comparison.");
                    result = COMPILE_ERROR;
                    break;
                }
                write_comparison(output_file, "jg", type_stack, &type_stack_size);
                break;
            case OP_LESS:
                if (type_stack_size < 2) {
                    runtime_error("Type stack underflow in comparison.");
                    result = COMPILE_ERROR;
                    break;
                }
                write_comparison(output_file, "jl", type_stack, &type_stack_size);
                break;
            case OP_NOT:
                if (type_stack_size < 1) {
                    runtime_error("Type stack underflow in OP_NOT.");
                    result = COMPILE_ERROR;
                    break;
                }

                ValueType a = type_stack[--type_stack_size];

                // A operação NOT só pode ser aplicada a tipos booleanos.
                if (a != VAL_BOOL) {
                    runtime_error("OP_NOT requires a boolean type.");
                    result = COMPILE_ERROR;
                    break;
                }

                fprintf(output_file, "    ;; -- OP_NOT --\n");
                fprintf(output_file, "    pop rax\n");  // Pegue o valor do topo da pilha

                // Inverte o valor booleano: se for 0 (false), vira 1 (true); se for 1 (true), vira 0 (false)
                fprintf(output_file, "    cmp rax, 0\n");     // Compara com 0
                fprintf(output_file, "    mov rbx, 0\n");     // Prepara rbx com 0 (false)
                fprintf(output_file, "    mov rcx, 1\n");     // Prepara rcx com 1 (true)
                fprintf(output_file, "    cmove rax, rcx\n"); // Se rax == 0, mova 1 para rax
                fprintf(output_file, "    cmovne rax, rbx\n"); // Se rax != 0, mova 0 para rax
                fprintf(output_file, "    push rax\n");        // Empurra o valor de volta na pilha

                type_stack[type_stack_size++] = VAL_BOOL; // O resultado será booleano
                break;
            case OP_ADD:
            case OP_SUBTRACT:
            case OP_MULTIPLY:
            case OP_DIVIDE: {
                if (type_stack_size < 2) {
                    runtime_error("Type stack underflow in arithmetic operation.");
                    result = COMPILE_ERROR;
                    break;
                }
                ValueType b = type_stack[--type_stack_size];
                ValueType a = type_stack[--type_stack_size];
                if (a != b) {
                    runtime_error("Operands must be of the same type");
                    result = COMPILE_ERROR;
                    break;
                }
                const char *op = get_arithmetic_op(instruction, a);
                if (!op) {
                    runtime_error("Unknown arithmetic operation.");
                    result = COMPILE_ERROR;
                    break;
                }
                fprintf(output_file, "    ;; Arithmetic operation: %s\n", op);
                fprintf(output_file, "    pop rbx\n");
                fprintf(output_file, "    pop rax\n");
                if (a == VAL_INT || a == VAL_UINT) {
                    const char *operand = (a == VAL_INT) ? "signed" : "unsigned";

                    if (!strcmp(op, "div") || !strcmp(op, "idiv")) {
                        // Divisão: diferenciando entre signed e unsigned
                        fprintf(output_file, "    ;; %s division\n", operand);
                        fprintf(output_file, "    %s rbx\n", op);  // 'div' ou 'idiv'
                    } else if (!strcmp(op, "imul")) {
                        // Multiplicação: diferenciando entre signed e unsigned
                        fprintf(output_file, "    ;; signed multiplication\n");
                        fprintf(output_file, "    imul rax, rbx\n");  //'imul'
                    } else if(!strcmp(op, "mul")) {
                        fprintf(output_file, "    ;; unsigned multiplication\n");
                        fprintf(output_file, "    mul rbx\n");
                    } else if (!strcmp(op, "add") || !strcmp(op, "sub")) {
                        // Operações comuns para adição e subtração (tanto signed quanto unsigned)
                        fprintf(output_file, "    ;; %s operation\n", op);
                        fprintf(output_file, "    %s rax, rbx\n", op);
                    }
                    fprintf(output_file, "    push rax\n");  // Empurra o resultado de volta na pilha
                } else if(a == VAL_FLOAT) {
                    if(!strcmp(op, "addsd")) {
                        fprintf(output_file, "    ;; float addition\n");
                        fprintf(output_file, "    movq xmm0, rax\n");
                        fprintf(output_file, "    movq xmm1, rbx\n");
                        fprintf(output_file, "    addsd xmm0, xmm1\n");
                        fprintf(output_file, "    movq rax, xmm0\n");
                    } else if(!strcmp(op, "subsd")) {
                        fprintf(output_file, "    movq xmm0, rax\n");
                        fprintf(output_file, "    movq xmm1, rbx\n");
                        fprintf(output_file, "    subsd xmm0, xmm1\n");
                        fprintf(output_file, "    movq rax, xmm0\n");
                    } else if(!strcmp(op, "mulsd")) {
                        fprintf(output_file, "    movq xmm0, rax\n");
                        fprintf(output_file, "    movq xmm1, rbx\n");
                        fprintf(output_file, "    mulsd xmm0, xmm1\n");
                        fprintf(output_file, "    movq rax, xmm0\n");
                    } else if(!strcmp(op, "divsd")) {
                        fprintf(output_file, "    movq xmm0, rax\n");
                        fprintf(output_file, "    movq xmm1, rbx\n");
                        fprintf(output_file, "    divsd xmm0, xmm1\n");
                        fprintf(output_file, "    movq rax, xmm0\n");
                    }
                    fprintf(output_file, "    push rax\n");
                }
                type_stack[type_stack_size++] = a;
                break;
            }
            case OP_RETURN:
                if(type_stack_size > 0) {
                    ValueType a = type_stack[--type_stack_size];
                    switch(a) {
                        case VAL_INT:
                            fprintf(output_file, "    ;; -- Print Int\n");
                            fprintf(output_file, "    pop rdi\n");
                            fprintf(output_file, "    call __built_in_print_int\n");
                            break;
                        case VAL_UINT:
                            fprintf(output_file, "    ;; -- Print Uint\n");
                            fprintf(output_file, "    pop rdi\n");
                            fprintf(output_file, "    call __built_in_print_uint\n");
                            break;
                        case VAL_BOOL:
                            fprintf(output_file, "    ;; -- Print bool\n");
                            fprintf(output_file, "    pop rdi\n");
                            fprintf(output_file, "    call __built_in_print_bool\n");
                            break;
                        case VAL_FLOAT:
                            fprintf(output_file, "    ;; -- Print Float\n");
                            fprintf(output_file, "    pop rdi\n");
                            fprintf(output_file, "    call __built_in_print_float\n");
                            break;
                        default:
                            fprintf(stderr, "%s:%s:%d Unreachable", __FILE__, __func__, __LINE__);

                    }
                }
                fprintf(output_file, "    ;; Return from main\n");
                write_syscall_exit(output_file);
                goto cleanup;
            default:
                fprintf(stderr, "Unrecognized opcode: %d\n", instruction);
                result = COMPILE_ERROR;
                goto cleanup;
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT

cleanup:

    fprintf(output_file, "segment writable\n");
    for(uint64_t i = 0; i < floats_size; i++) {
        fprintf(output_file, "    double_val_%zu dq %lf\n", i, floats[i]);
    }
    fprintf(output_file, "    factor dq 1000000.0\n");
    fprintf(output_file, "    minus_str db \"-\", 0\n");
    fprintf(output_file, "    dot_str db \".\", 0\n");
    fprintf(output_file, "    zeros db \"000000\", 0\n");
    fprintf(output_file, "    true_str db \"true\", 0\n");
    fprintf(output_file, "    true_str_len = $-true_str\n");
    fprintf(output_file, "    false_str db \"false\", 0\n");
    fprintf(output_file, "    false_str_len = $-false_str\n");


    fclose(output_file);
    free(output_file_path);
    free(type_stack);
    return result;
}

CompileResult compile_to_native(const char *source, const char *output_path) {
    Chunk chunk;
    init_chunk(&chunk);
    if (!compile(source, &chunk)) {
        free_chunk(&chunk);
        return COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    CompileResult result = native_compile(output_path);
    return result;
}

void push(Value value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

Value pop() {
    vm.stack_top--;
    return *vm.stack_top;
}