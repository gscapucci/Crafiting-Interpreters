#include "../include/lox.h"


char *file_to_str(const char *path) {
    FILE *file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *str = malloc(size + 1);
    fread(str, size, 1, file);
    str[size] = 0;

    fclose(file);

    return str;
}

void lox_main(int argc, char **argv) {
    Lox lox = (Lox) {
        .argc = argc,
        .argv = argv,
        .file = NULL,
        .hasError = false,
    };
    if(argc > 2) {
        printf("Usage: lox [script]\n");
        exit(64);
    } else if(argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }
}

void run_file(const char *path) {
    lox.file = file_to_str(path);
    run(lox.file);
    free(lox.file);
    lox.file = NULL;
    if(lox.hasError) {
        exit(65);
    }
}

void run_prompt() {
    char line[256] = {0};
    while(true) {
        memset(line, 0, 256);
        printf("> ");
        fflush(stdout);
        if(fgets(line, 256, stdin) == NULL) {
            fflush(stdin);
            break;
        }
        run(line);
        lox.hasError = false;
    }
}

void run(const char *source) {
    Scanner scanner = create_scanner(source);
    TokenVec tokens = scan_tokens(&scanner);
    for(uint64_t i = 0; i < tokens.size; i++) {
        const Token *aux = token_vec_at(&tokens, i);
        char *str = token_to_str(aux);
        printf("%s\n", str);
        free(str);
    }
    delete_scanner(scanner);
}