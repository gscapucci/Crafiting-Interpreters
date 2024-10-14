#include "assembly.h"

#include <stdarg.h>

static void convert_uint64_to_float64() {

}
static void print_float_asm(FILE *file) {
    const char *func =  "__built_in_print_float:\n"
                        "    push    rbp\n"
                        "    mov     rbp, rsp\n"
                        "    sub     rsp, 48\n"
                        "    movsd   QWORD [rbp-40], xmm0\n"
                        "    pxor    xmm0, xmm0\n"
                        "    comisd  xmm0, QWORD [rbp-40]\n"
                        "    seta    al\n"
                        "    movzx   eax, al\n"
                        "    mov     DWORD [rbp-4], eax\n"
                        "    cmp     DWORD [rbp-4], 0\n"
                        "    je      .L6\n"
                        "    movsd   xmm0, QWORD [rbp-40]\n"
                        "    movq    xmm1, QWORD [minus_str]\n"
                        "    xorpd   xmm0, xmm1\n"
                        "    movsd   QWORD [rbp-40], xmm0\n"
                        "    mov     rdx, 1\n"
                        "    mov     rsi, minus_str\n"
                        "    mov     rdi, 1\n"
                        "    mov     rax, 1\n"
                        "    syscall\n"
                        ".L6:\n"
                        "    movsd   xmm0, QWORD [rbp-40]\n"
                        "    cvttsd2si       rax, xmm0\n"
                        "    mov     QWORD [rbp-16], rax\n"
                        "    mov     rax, QWORD [rbp-16]\n"
                        "    mov     rdx, rax\n"
                        "    neg     rdx\n"
                        "    cmovns  rax, rdx\n"
                        "    mov     rdi, rax\n"
                        "    call    __built_in_print_int\n"
                        "    mov     rdx, 1\n"
                        "    mov     rsi, dot_str\n"
                        "    mov     rdi, 1\n"
                        "    mov     rax, 1\n"
                        "    syscall\n"
                        "    pxor    xmm1, xmm1\n"
                        "    cvtsi2sd        xmm1, QWORD [rbp-16]\n"
                        "    movsd   xmm0, QWORD [rbp-40]\n"
                        "    subsd   xmm0, xmm1\n"
                        "    movsd   QWORD [rbp-24], xmm0\n"
                        "    movsd   xmm1, QWORD [rbp-24]\n"
                        "    movsd   xmm0, QWORD [factor]\n"
                        "    mulsd   xmm0, xmm1\n"
                        "    cvttsd2si       rax, xmm0\n"
                        "    mov     QWORD [rbp-32], rax\n"
                        "    mov     rax, QWORD [rbp-32]\n"
                        "    mov     rdx, rax\n"
                        "    neg     rdx\n"
                        "    cmovns  rax, rdx\n"
                        "    mov     rdi, rax\n"
                        "    cmp    rdi, 0\n"
                        "    je      .print_zeros\n"
                        "    call    __built_in_print_int\n"
                        "    nop\n"
                        "    leave\n"
                        "    ret\n"
                        ".print_zeros:\n"
                        "    mov rdx, 6\n"
                        "    mov rsi, zeros\n"
                        "    mov rdi, 1\n"
                        "    mov rax, 1\n"
                        "    syscall\n"
                        "    nop\n"
                        "    leave\n"
                        "    ret\n";
    fprintf(file, "%s", func);
}

static void print_int64_asm(FILE *file) {
    const char *func =  "__built_in_print_int:\n"
                        "    mov     rcx, rdi\n"
                        "    sub     rsp, 40\n"
                        "    mov     r11, rdi\n"
                        "    mov     r9, -3689348814741910323\n"
                        "    neg     rcx\n"
                        "    lea     r8, [rsp+19]\n"
                        "    cmovs   rcx, rdi\n"
                        "    mov     rdi, 20\n"
                        ".L3:\n"
                        "    mov     rax, rcx\n"
                        "    mov     r10d, edi\n"
                        "    sub     r8, 1\n"
                        "    mul     r9\n"
                        "    mov     rax, rcx\n"
                        "    sub     rdi, 1\n"
                        "    shr     rdx, 3\n"
                        "    lea     rsi, [rdx+rdx*4]\n"
                        "    add     rsi, rsi\n"
                        "    sub     rax, rsi\n"
                        "    add     eax, 48\n"
                        "    mov     BYTE [r8+1], al\n"
                        "    mov     rax, rcx\n"
                        "    mov     rcx, rdx\n"
                        "    cmp     rax, 9\n"
                        "    ja      .L3\n"
                        "    movsx   rax, di\n"
                        "    test    r11, r11\n"
                        "    jns     .L4\n"
                        "    lea     rdi, [r10-2]\n"
                        "    movsx   rax, di\n"
                        "    mov     BYTE [rsp+rax], 45\n"
                        ".L4:\n"
                        "    mov     edx, 20\n"
                        "    lea     rsi, [rsp+rax]\n"
                        "    sub     edx, edi\n"
                        "    mov     rdi, 1\n"
                        "    movsx   rdx, dx\n"
                        "    mov     rax, 1\n"
                        "    syscall\n"
                        "    add     rsp, 40\n"
                        "    ret\n";
    fprintf(file, "%s", func);
}

static void call_print_int64_asm(FILE *file) {
    const char *call =  "    pop rax\n"
                        "    mov rdi, rax\n"
                        "    call __built_in_print_int\n";
    fprintf(file, "%s", call);
}

static void print_uint64_asm(FILE *file) {
    const char *func =  "__built_in_print_uint:\n"
                        "    sub     rsp, 40\n"
                        "    mov     ecx, 19\n"
                        "    mov  r9, -3689348814741910323\n"
                        ".L2:\n"
                        "    mov     rax, rdi\n"
                        "    movsx   r8, cx\n"
                        "    mul     r9\n"
                        "    mov     rax, rdi\n"
                        "    shr     rdx, 3\n"
                        "    lea     rsi, [rdx+rdx*4]\n"
                        "    add     rsi, rsi\n"
                        "    sub     rax, rsi\n"
                        "    add     eax, 48\n"
                        "    mov     BYTE [rsp+rcx], al\n"
                        "    mov     rax, rdi\n"
                        "    sub     rcx, 1\n"
                        "    mov     rdi, rdx\n"
                        "    cmp     rax, 9\n"
                        "    ja      .L2\n"
                        "    mov     edx, 20\n"
                        "    lea     rsi, [rsp+r8]\n"
                        "    mov     rdi, 1\n"
                        "    sub     edx, r8d\n"
                        "    movsx   rdx, dx\n"
                        "    mov     rax, 1\n"
                        "    syscall\n"
                        "    add     rsp, 40\n"
                        "    ret\n";
    fprintf(file, "%s", func);
}

static void print_bool_asm(FILE *file) {
    const char *func =  "__built_in_print_bool:\n"
                        "    push    rbp\n"
                        "    mov     rbp, rsp\n"
                        "    sub     rsp, 16\n"
                        "    mov     rsi, false_str\n"
                        "    mov     rdx, false_str_len\n"
                        "    cmp     rdi, 0\n"
                        "    mov     rax, true_str\n"
                        "    cmovne  rsi, rax\n"
                        "    mov     rax, true_str_len\n"
                        "    cmovne  rdx, rax\n"
                        "    mov     rax, 1\n"
                        "    mov     rdi, 1\n"
                        "    syscall\n"
                        "    nop\n"
                        "    leave\n"
                        "    ret\n";
    fprintf(file, "%s", func);
}

void fprintf_asm_aux_functions(FILE *file) {
    print_float_asm(file);
    print_int64_asm(file);
    print_uint64_asm(file);
    print_bool_asm(file);
}

void write_asm_header(FILE *file) {
    const char *fasm_header =   "format ELF64 executable 3\n"
                                "use64\n"
                                "segment executable\n"
                                "entry main\n";
    fprintf(file, "%s", fasm_header);
}
void write_syscall_exit(FILE *file) {
    fprintf(file, "    mov rax, 60  ; syscall for exit\n");
    fprintf(file, "    xor rdi, rdi ; exit status 0\n");
    fprintf(file, "    syscall\n");
}
void write_constant(FILE *file, Value constant, uint64_t *float_count) {
    switch (constant.type) {
        case VAL_UINT:
            fprintf(file, "    ;; Push uint64_t constant\n");
            fprintf(file, "    mov rax, %lu\n", constant.as.uint_number);
            break;
        case VAL_INT:
            fprintf(file, "    ;; Push int64_t constant\n");
            fprintf(file, "    mov rax, %ld\n", constant.as.int_number);
            break;
        case VAL_BOOL:
            fprintf(file, "    ;; Push bool constant\n");
            fprintf(file, "    mov rax, %d\n", constant.as.boolean ? 1 : 0);
            break;
        case VAL_FLOAT:
            fprintf(file, "    ;; Push float constant\n");
            fprintf(file, "    movq xmm0, [double_val_%zu]\n", *float_count);
            fprintf(file, "    movq rax, xmm0\n");
            *float_count += 1;
            break;
        default:
            return;
    }
    fprintf(file, "    push rax\n");
}
const char *get_arithmetic_op(uint8_t instruction, ValueType type) {
    if (type == VAL_INT || type == VAL_UINT) {
        switch (instruction) {
            case OP_ADD: return "add";
            case OP_SUBTRACT: return "sub";
            case OP_MULTIPLY: return (type == VAL_INT) ? "imul" : "mul";
            case OP_DIVIDE: return (type == VAL_INT) ? "idiv" : "div";
            default: return NULL;
        }
    }
    if(type == VAL_FLOAT) {
        switch (instruction) {
            case OP_ADD: return "addsd";
            case OP_SUBTRACT: return "subsd";
            case OP_MULTIPLY: return "mulsd";
            case OP_DIVIDE: return "divsd";
            default: return NULL;
        }
    }
    return NULL;
}
void write_comparison(FILE *file, const char *cmp_op, ValueType *type_stack, uint64_t *type_stack_size) {
    ValueType b = type_stack[--(*type_stack_size)];
    ValueType a = type_stack[--(*type_stack_size)];
    if (a != b) {
        fprintf(file, "    ;; Comparison with different types, result = false\n");
        fprintf(file, "    push 0\n");
    } else {
        fprintf(file, "    ;; %s comparison\n", cmp_op);
        fprintf(file, "    pop rbx\n");
        fprintf(file, "    pop rax\n");
        fprintf(file, "    cmp rax, rbx\n");
        fprintf(file, "    %s al\n", cmp_op);
        fprintf(file, "    movzx rax, al\n");
        fprintf(file, "    push rax\n");
    }
    type_stack[(*type_stack_size)++] = VAL_BOOL;
}