#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembly.h"
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void repl() {
    char line[1024];
    for(;;) {
        printf("> ");
        if(!fgets(line, sizeof(line), stdin) || !strcmp(line, "q")) {
            printf("\n");
            break;
        }
        interpret(line);
    }
}

static char *read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if(file == NULL) {
        fprintf(stderr, "Could not open file\"%s\".\n", path);
        exit(74);
    }
    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);
    if(buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if(bytes_read < file_size) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

static void run_file(const char *path) {
    char *source = read_file(path);
    InterpretResult result = interpret(source);
    free(source);

    if(result == INTERPRET_COMPILE_ERROR) exit(10);
    if(result == INTERPRET_RUNTIME_ERROR) exit(20);
}

static void compile_file_to_linux_x64_64(const char *path) {
    char *source = read_file(path);

    CompileResult result = compile_to_native(source, path);
    free(source);

    if (result == COMPILE_ERROR) exit(30);

    // printf("== Assembly output ====\n");

    char path1[500] = {0};
    size_t path_len = strlen(path);
    size_t ext_len = strlen(".lox");

    // Verifica se o path é suficientemente longo para remover ".lox"
    if (path_len > ext_len) {
        snprintf(path1, sizeof(path1), "%.*s.fasm", (int)(path_len - ext_len), path);
    } else {
        fprintf(stderr, "Invalid file path: %s\n", path);
        exit(1);
    }

    // char *file = read_file(path1);
    // printf("%s\n", file);
    // free(file);

    char command[1000] = {0};
    snprintf(command, sizeof(command), "fasm %s > /dev/null", path1);
    // fprintf(stderr, "DEBUG: %s\n", command);
    if (system(command)) {
        fprintf(stderr, "Compilation fail\n");
        exit(1);
    }

    // snprintf(command, sizeof(command), "%.*s", (int)(strlen(path1) - strlen(".fasm")), path1);
    // fprintf(stderr, "DEBUG: %s\n", command);
    // if (system(command)) {
    //     fprintf(stderr, "Execution fail\n");
    //     exit(1);
    // }
}

static void print_usage() {
    fprintf(stderr, "Usage: ./clox [option] <path>\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "    -i                                 run interative repl.\n");
    fprintf(stderr, "    -r, -run                           interpret the file.\n");
    fprintf(stderr, "    -c, -compile [-t, -target]         compile the file\n");
    fprintf(stderr, "            target: linux_x86_64\n");
    exit(64);
}

static void run_opt(const char *path) {
    if(vm.options & OPT_INTERPRET) {
        run_file(path);
    }
    if(vm.options & OPT_COMPILE_LINUX_X86_64) {
        compile_file_to_linux_x64_64(path);
    }
    if(vm.options & OPT_COMPILE_WINDOWS) {
        fprintf(stderr, "TODO: Uninplemented compilation for windows\n");
        return;
    }
    if(vm.options & OPT_RUN_EXECUTABLE) {
        char command[255] = {0};
        memcpy(command, path, strlen(path) - strlen(".lox"));
        if(system(command)) {
            fprintf(stderr, "ERROR");
            return;
        }
    }
}

int main(int argc, char **argv) {
    init_VM();
    switch(argc) {
        case 1:
            print_usage();
            break;
        case 2: 
            {
                argv++;
                if(!strcmp(*argv, "-i")) {
                    repl();
                    goto exit;
                } else {
                    print_usage();
                }
            }
            break;
        case 3:
            argv++;
            if(!strcmp(*argv, "-c") || !strcmp(*argv, "-compile")) {
                #ifdef _WIN64
                    fprintf(stderr, "Compilation to windows not suported yet.\n");
                    goto exit;
                #elif __linux__
                    vm.options |= OPT_COMPILE_LINUX_X86_64;
                #else
                    fprintf(stderr, "OS not supported yet.\n");
                #endif
            } else if(!strcmp(*argv, "-r") || !strcmp(*argv, "-run")) {
                vm.options |= OPT_INTERPRET;
            } else if(!strcmp(*argv, "-cr") || !strcmp(*argv, "-compiler-run")) {
                #ifdef _WIN64
                    fprintf(stderr, "Compilation to windows not suported yet.\n");
                    goto exit;
                #elif __linux__
                    vm.options |= OPT_COMPILE_LINUX_X86_64;
                    vm.options |= OPT_RUN_EXECUTABLE;
                #else
                    fprintf(stderr, "OS not supported yet.\n");
                #endif
            } else {
                print_usage();
            }
            break;
        default:
            print_usage();
            goto exit;
    }
    argv++;
    run_opt(*argv);
exit:
    free_VM();
    return 0;
}