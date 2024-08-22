#include "lox.h"
#include "scanner.h"
#include "error.h"
#include "macro.h"
#include "token_vec.h"  
#include "parser.h"
#include "astprinter.h"
#include "interpreter.h"



Lox *new_lox(int argc, char **argv) {
    Lox *lox = (Lox *)malloc(sizeof(Lox));
    lox->argc = argc;
    lox->argv = argv;
    lox->had_error = false;
    lox->had_runtime_error = false;
    return lox;
}

void free_lox(Lox *lox) {
    if(!lox) return;
    free(lox);
}

int lox_main(Lox *lox) {
    if(!lox) return 1;
    if(lox->argc > 2) {
        printf("Usage: clox [script]\n");
        return 64;
    }
    if(lox->argc == 2) {
        return run_file(lox);
    }
    return run_prompt(lox);
}

int run_file(Lox *lox) {
    char *file = read_file(lox->argv[1]);
    if(!file) return 1;
    run(lox, file);
    if(lox->had_error) {
        exit(65);
    }
    if(lox->had_runtime_error) {
        exit(70);
    }
    free(file);
    return 0;
}

int run_prompt(Lox *lox) {
    char input[256] = {0};
    int ret = 0;
    for(;;) {
        printf("> ");
        fflush(stdout);
        if(!fgets(input, 255, stdin)) break;
        ret = run(lox, input);
    }
    return ret;
}

// void println_stmt(const char *pad, Stmt stmt) {
//     switch (stmt.type) {
//         case StmtTypeBlock:
//             printf("%s[\n", pad);
//             for(uint64_t i = 0; i < stmt.block.statements.size; i++) {
//                 char padd[100] = {0};
//                 strcpy(padd, pad);
//                 strcat(padd, "\t");
//                 println_stmt(padd, stmt.block.statements.stmts[i]);
//             }
//             printf("%s]\n", pad);
//             break;
//         case StmtTypeExpression:
//             printf("%sExpression\n", pad);
//             break;
//         case StmtTypePrint:
//             printf("%sPrint\n", pad);
//             break;
//         case StmtTypeVar:
//             printf("%sVar\n", pad);
//             break;
//     }
// }

int run(Lox *lox, char *source) {
    Scanner scan = create_scanner(source);
    TokenVec tk_vec = scan_tokens(lox, &scan);
    Parser parser = create_parser(lox, tk_vec);
    Interpreter interpreter = create_interpreter(lox);



    StmtVec stmts = parse(&parser);
    // if(expr != NULL) {
    //     char *str = print(*expr);
    //     printf("%s\n", str);fflush(stdout);
    //     free(str);
    //     free_expr_tree(expr);
    // }
    if(stmts.size == 0) {
        lox->had_error = true;
        return -1;
    }
    // for(uint64_t i = 0; i < stmts.size; i++) {
    //     println_stmt("", stmts.stmts[i]);
    // }
    interpret(&interpreter, stmts);
    delete_interpreter(&interpreter);
    delete_stmt_vec(&stmts);
    delete_scanner(&scan);
    delete_token_vec(&tk_vec);
    return 0;
}

char *read_file(const char *path) {
    FILE *file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_str = (char *)malloc(size + 1);
    memset(file_str, 0, size + 1);
    uint64_t ret = fread(file_str, size, 1, file);
    if(ret == 0) {
        exit(1);
    }
    fclose(file);
    return file_str;
}