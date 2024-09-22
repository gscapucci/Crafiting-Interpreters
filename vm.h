#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_MAX 256

typedef enum {
    OPT_NONE = 1 << 0,
    OPT_INTERPRET = 1 << 1,
    OPT_COMPILE_LINUX_X86_64 = 1 << 3,
    OPT_COMPILE_WINDOWS = 1 << 4,
} Options;

typedef struct {
    Chunk *chunk;
    uint8_t *ip;
    Value stack[STACK_MAX];
    Value *stack_top;
    Table strings;
    Obj *objects;
    Options options;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

typedef enum {
    COMPILE_OK,
    COMPILE_ERROR
} CompileResult;

extern VM vm;

void init_VM();
void free_VM();
InterpretResult interpret(const char *source);
CompileResult compile_to_native(const char *source, const char *output_path);
void push(Value value);
Value pop();


#endif /* CLOX_VM_H */