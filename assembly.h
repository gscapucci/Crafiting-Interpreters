#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "vm.h"

#include <stdio.h>

void fprintf_asm_aux_functions(FILE *file);

void write_asm_header(FILE *file);
void write_syscall_exit(FILE *file);
void write_constant(FILE *file, Value constant, uint64_t *float_count);
const char *get_arithmetic_op(uint8_t instruction, ValueType type);
void write_comparison(FILE *file, const char *cmp_op, ValueType *type_stack, uint64_t *type_stack_size);



#endif /* ASSEMBLY_H */