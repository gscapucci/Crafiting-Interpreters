#include "assembly.h"

#include <stdarg.h>

static void convert_uint64_to_float64() {

}
static void print_float_asm(FILE *file) {
const char *func =  "__built_in_print_float:\n"
                    "    sub     rsp, 520\n"
                    "    mov     r9, rdi\n"
                    "    xor     eax, eax\n"
                    "    mov     ecx, 64\n"
                    "    mov     rdi, rsp\n"
                    "    mov     rdx, r9\n"
                    "    mov     r10, rsp\n"
                    "    rep stosq\n"
                    "    not     rdx\n"
                    "    mov     rax, 9218868437227405312\n"
                    "    test    rdx, rax\n"
                    "    je      .L25\n"
                    "    movq    xmm0, r9\n"
                    "    test    r9, r9\n"
                    "    js      .L26\n"
                    "    movsd   xmm1, QWORD [rip]\n"
                    "    comisd  xmm0, xmm1\n"
                    "    jnb     .L7\n"
                    ".L28:\n"
                    "    cvttsd2si       rcx, xmm0\n"
                    "    test    rcx, rcx\n"
                    "    js      .L9\n"
                    ".L27:\n"
                    "    pxor    xmm2, xmm2\n"
                    "    cvtsi2sd        xmm2, rcx\n"
                    ".L10:\n"
                    "    subsd   xmm0, xmm2\n"
                    "    mulsd   xmm0, QWORD [rip]\n"
                    "    comisd  xmm0, xmm1\n"
                    "    jnb     .L11\n"
                    "    cvttsd2si       rdi, xmm0\n"
                    ".L12:\n"
                    "    mov     r8, -3689348814741910323\n"
                    "    mov     rax, rdi\n"
                    "    mov     BYTE [rsp+500], 46\n"
                    "    mul     r8\n"
                    "    shr     rdx, 3\n"
                    "    lea     rax, [rdx+rdx*4]\n"
                    "    mov     rsi, rdx\n"
                    "    add     rax, rax\n"
                    "    sub     rdi, rax\n"
                    "    mov     rax, rdx\n"
                    "    mul     r8\n"
                    "    add     edi, 48\n"
                    "    mov     BYTE [rsp+510], dil\n"
                    "    shr     rdx, 3\n"
                    "    lea     rax, [rdx+rdx*4]\n"
                    "    mov     rdi, rdx\n"
                    "    add     rax, rax\n"
                    "    sub     rsi, rax\n"
                    "    mov     rax, rdx\n"
                    "    mul     r8\n"
                    "    add     esi, 48\n"
                    "    mov     BYTE [rsp+509], sil\n"
                    "    mov     rsi, rdx\n"
                    "    shr     rsi, 3\n"
                    "    lea     rax, [rsi+rsi*4]\n"
                    "    add     rax, rax\n"
                    "    sub     rdi, rax\n"
                    "    mov     rax, rsi\n"
                    "    mul     r8\n"
                    "    add     edi, 48\n"
                    "    mov     BYTE [rsp+508], dil\n"
                    "    shr     rdx, 3\n"
                    "    lea     rax, [rdx+rdx*4]\n"
                    "    mov     rdi, rdx\n"
                    "    add     rax, rax\n"
                    "    sub     rsi, rax\n"
                    "    mov     rax, rdx\n"
                    "    mul     r8\n"
                    "    add     esi, 48\n"
                    "    mov     BYTE [rsp+507], sil\n"
                    "    mov     rsi, rdx\n"
                    "    shr     rsi, 3\n"
                    "    lea     rax, [rsi+rsi*4]\n"
                    "    add     rax, rax\n"
                    "    sub     rdi, rax\n"
                    "    mov     rax, rsi\n"
                    "    mul     r8\n"
                    "    add     edi, 48\n"
                    "    mov     BYTE [rsp+506], dil\n"
                    "    mov     rdi, rdx\n"
                    "    shr     rdi, 3\n"
                    "    lea     rax, [rdi+rdi*4]\n"
                    "    add     rax, rax\n"
                    "    sub     rsi, rax\n"
                    "    mov     rax, rdi\n"
                    "    mul     r8\n"
                    "    add     esi, 48\n"
                    "    mov     BYTE [rsp+505], sil\n"
                    "    mov     rsi, rdx\n"
                    "    shr     rsi, 3\n"
                    "    lea     rax, [rsi+rsi*4]\n"
                    "    add     rax, rax\n"
                    "    sub     rdi, rax\n"
                    "    mov     rax, rsi\n"
                    "    mul     r8\n"
                    "    add     edi, 48\n"
                    "    mov     BYTE [rsp+504], dil\n"
                    "    shr     rdx, 3\n"
                    "    lea     rax, [rdx+rdx*4]\n"
                    "    mov     rdi, rdx\n"
                    "    add     rax, rax\n"
                    "    sub     rsi, rax\n"
                    "    mov     rax, rdx\n"
                    "    mul     r8\n"
                    "    add     esi, 48\n"
                    "    mov     BYTE [rsp+503], sil\n"
                    "    mov     rsi, rdx\n"
                    "    shr     rsi, 3\n"
                    "    lea     rax, [rsi+rsi*4]\n"
                    "    add     rax, rax\n"
                    "    sub     rdi, rax\n"
                    "    mov     rax, rsi\n"
                    "    mul     r8\n"
                    "    add     edi, 48\n"
                    "    mov     BYTE [rsp+502], dil\n"
                    "    shr     rdx, 3\n"
                    "    lea     rax, [rdx+rdx*4]\n"
                    "    add     rax, rax\n"
                    "    sub     rsi, rax\n"
                    "    add     esi, 48\n"
                    "    mov     BYTE [rsp+501], sil\n"
                    "    test    rcx, rcx\n"
                    "    jne     .L18\n"
                    "    mov     BYTE [rsp+499], 48\n"
                    "    mov     esi, 499\n"
                    ".L14:\n"
                    "    test    r9, r9\n"
                    "    jns     .L15\n"
                    "    mov     BYTE [rsp-1+rsi], 45\n"
                    "    sub     rsi, 1\n"
                    ".L15:\n"
                    "    mov     edx, 511\n"
                    "    sub     rdx, rsi\n"
                    ".L23:\n"
                    "    add     rsi, r10\n"
                    "    mov     edi, 1\n"
                    "    mov     rax, 1\n"
                    "    mov     rdi, 1\n"
                    "    syscall\n"
                    "    add     rsp, 520\n"
                    "    ret\n"
                    ".L7:\n"
                    "    movapd  xmm2, xmm0\n"
                    "    subsd   xmm2, xmm1\n"
                    "    cvttsd2si       rcx, xmm2\n"
                    "    btc     rcx, 63\n"
                    "    test    rcx, rcx\n"
                    "    jns     .L27\n"
                    ".L9:\n"
                    "    mov     rax, rcx\n"
                    "    mov     rdx, rcx\n"
                    "    pxor    xmm2, xmm2\n"
                    "    shr     rax, 1\n"
                    "    and     edx, 1\n"
                    "    or      rax, rdx\n"
                    "    cvtsi2sd        xmm2, rax\n"
                    "    addsd   xmm2, xmm2\n"
                    "    jmp     .L10\n"
                    ".L26:\n"
                    "    xorpd   xmm0, [rip]\n"
                    "    movsd   xmm1, QWORD [rip]\n"
                    "    comisd  xmm0, xmm1\n"
                    "    jnb     .L7\n"
                    "    jmp     .L28\n"
                    ".L18:\n"
                    "    mov     esi, 500\n"
                    ".L13:\n"
                    "    mov     rax, rcx\n"
                    "    sub     rsi, 1\n"
                    "    mul     r8\n"
                    "    mov     rax, rcx\n"
                    "    shr     rdx, 3\n"
                    "    lea     rdi, [rdx+rdx*4]\n"
                    "    add     rdi, rdi\n"
                    "    sub     rax, rdi\n"
                    "    add     eax, 48\n"
                    "    mov     BYTE [r10+rsi], al\n"
                    "    mov     rax, rcx\n"
                    "    mov     rcx, rdx\n"
                    "    cmp     rax, 9\n"
                    "    ja      .L13\n"
                    "    jmp     .L14\n"
                    ".L11:\n"
                    "    subsd   xmm0, xmm1\n"
                    "    cvttsd2si       rdi, xmm0\n"
                    "    btc     rdi, 63\n"
                    "    jmp     .L12\n"
                    ".L25:\n"
                    "    mov     rax, 4503599627370495\n"
                    "    test    r9, rax\n"
                    "    je      .L29\n"
                    "    mov     edx, 24910\n"
                    "    mov     BYTE [rsp+510], 78\n"
                    "    mov     esi, 508\n"
                    "    mov     WORD [rsp+508], dx\n"
                    "    mov     edx, 3\n"
                    "    jmp     .L23\n"
                    ".L29:\n"
                    "    sar     r9, 63\n"
                    "    mov     eax, 28233\n"
                    "    mov     edx, 4\n"
                    "    mov     BYTE [rsp+510], 102\n"
                    "    and     r9d, 2\n"
                    "    mov     WORD [rsp+508], ax\n"
                    "    mov     esi, 507\n"
                    "    add     r9d, 43\n"
                    "    mov     BYTE [rsp+507], r9b\n"
                    "    jmp     .L23\n";
    fprintf(file, "%s", func);
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
    const char *call =  "pop rax\n"
                        "mov rdi, rax\n"
                        "call __built_in_print_int\n";
    fprintf(file, "%s", call);
}

void fprintf_asm_aux_functions(FILE *file) {
    print_float_asm(file);
    print_int64_asm(file);
    print_uint64_asm(file);
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