#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

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
                printf("\n");
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
    CompileResult result = COMPILE_OK; // Variable to track success

    // FASM file header
    const char *fasm_header = 
        "format ELF64 executable 3\n"
        "use64\n"
        "segment executable\n"
        "entry main\n";
    
    fprintf(output_file, "%s", fasm_header);
    fprintf(output_file, "main:\n");

    for (;;) {
        if(type_stack_size >= type_stack_cap) {
            type_stack = GROW_ARRAY(ValueType, type_stack, type_stack_cap, type_stack_cap * 2);
        }
        uint8_t instruction = READ_BYTE();
        switch (instruction) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                switch (constant.type) {
                    case VAL_UINT:
                        fprintf(output_file, "    ;; Push uint64_t constant\n");
                        fprintf(output_file, "    mov rax, %lu\n", constant.as.uint_number);
                        fprintf(output_file, "    push rax\n");
                        break;
                    case VAL_INT:
                        fprintf(output_file, "    ;; Push int64_t constant\n");
                        fprintf(output_file, "    mov rax, %ld\n", constant.as.int_number);
                        fprintf(output_file, "    push rax\n");
                        break;
                    case VAL_BOOL:
                        fprintf(output_file, "    ;; Push bool constant\n");
                        fprintf(output_file, "    mov rax, %d\n", constant.as.boolean ? 1 : 0);
                        fprintf(output_file, "    push rax\n");
                        break;
                    default:
                        runtime_error("Unknown constant type in OP_CONSTANT.");
                        result = COMPILE_ERROR;
                        goto cleanup;
                }
                type_stack[type_stack_size++] = constant.type;
                break;
            }
            case OP_NIL:
                fprintf(output_file, "    ;;  -- OP_NIL --\n");
                fprintf(output_file, "    mov rax, 0\n");
                fprintf(output_file, "    push rax\n");
                type_stack[type_stack_size++] = VAL_NIL;
                break;
            case OP_TRUE:
                fprintf(output_file, "    ;;  -- OP_TRUE --\n");
                fprintf(output_file, "    mov rax, 1\n");
                fprintf(output_file, "    push rax\n");
                type_stack[type_stack_size++] = VAL_BOOL;
                break;
            case OP_FALSE:
                fprintf(output_file, "    ;;  -- OP_FALSE --\n");
                fprintf(output_file, "    mov rax, 0\n");
                fprintf(output_file, "    push rax\n");
                type_stack[type_stack_size++] = VAL_BOOL;
                break;
            case OP_EQUAL: {
                if (type_stack_size < 2) {
                    runtime_error("Type stack underflow in OP_EQUAL.");
                    result = COMPILE_ERROR;
                    goto cleanup;
                }
                ValueType b = type_stack[--type_stack_size];
                ValueType a = type_stack[--type_stack_size];
                if(a != b) {
                    fprintf(output_file, "    ;;  -- OP_EQUAL (DIFF TYPES) --\n");
                    fprintf(output_file, "    push 0\n");
                    break;
                }
                fprintf(output_file, "    ;;  -- OP_EQUAL --\n");
                fprintf(output_file, "    pop rbx\n");
                fprintf(output_file, "    pop rax\n");
                fprintf(output_file, "    cmp rax, rbx\n");
                fprintf(output_file, "    sete al\n");
                fprintf(output_file, "    movzx rax, al\n");
                fprintf(output_file, "    push rax\n");
                type_stack[type_stack_size++] = VAL_BOOL;
                break;
            }
            case OP_GREATER: {
                if (type_stack_size < 2) {
                    runtime_error("Type stack underflow in OP_GREATER.");
                    result = COMPILE_ERROR;
                    goto cleanup;
                }
                ValueType b = type_stack[--type_stack_size];
                ValueType a = type_stack[--type_stack_size];
                if(a != b) {
                    fprintf(output_file, "    ;;  -- OP_EQUAL (DIFF TYPES) --\n");
                    fprintf(output_file, "    push 0\n");
                    break;
                }
                fprintf(output_file, ";;  -- OP_GREATER --\n");
                fprintf(output_file, "    pop rbx\n");
                fprintf(output_file, "    pop rax\n");
                fprintf(output_file, "    cmp rax, rbx\n");
                fprintf(output_file, "    setg al\n");
                fprintf(output_file, "    movzx rax, al\n");
                fprintf(output_file, "    push rax\n");
                type_stack[type_stack_size++] = VAL_BOOL;
                break;
            }
            case OP_ADD:
            case OP_SUBTRACT:
            case OP_MULTIPLY:
            case OP_DIVIDE: {
                if (type_stack_size < 2) {
                    runtime_error("Type stack underflow in arithmetic operation.");
                    result = COMPILE_ERROR;
                    goto cleanup;
                }
                ValueType b = type_stack[--type_stack_size];
                ValueType a = type_stack[--type_stack_size];
                if(a != b) {
                    runtime_error("Operands must be of the same type");
                    result = COMPILE_ERROR;
                    goto cleanup;
                }
                const char *op = NULL;
                if (instruction == OP_ADD) op = "add";
                else if (instruction == OP_SUBTRACT) op = "sub";
                else if (instruction == OP_MULTIPLY) op = "imul";
                else if (instruction == OP_DIVIDE) op = "idiv";
                
                fprintf(output_file, ";;  -- OP_%s --\n", op);
                fprintf(output_file, "    pop rbx\n");
                fprintf(output_file, "    pop rax\n");
                fprintf(output_file, "    %s rax, rbx\n", op);
                fprintf(output_file, "    push rax\n");
                type_stack[type_stack_size++] = a;
                break;
            }
            case OP_RETURN:
                type_stack_size--;
                fprintf(output_file, "    mov rax, 60  ; syscall for exit\n");
                fprintf(output_file, "    xor rdi, rdi ; status 0\n");
                fprintf(output_file, "    syscall\n");
                goto cleanup;
            default:
                fprintf(stderr, "Unrecognized opcode: %d\n", instruction);
                result = COMPILE_ERROR;
                goto cleanup;
        }
    }

cleanup:
    fclose(output_file);
    free(output_file_path);
    free(type_stack);
    return result;

    #undef READ_BYTE
    #undef READ_CONSTANT
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