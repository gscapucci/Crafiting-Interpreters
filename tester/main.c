#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define ENDSWITH(str, suffix) (strcmp(&(str)[strlen(str) - strlen(suffix)], suffix) == 0)

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

static void usage() {
    fprintf(stderr, "Usage: ./clox [option]\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\trec, -r record the tests\n");
    fprintf(stderr, "\ttest, -t run the tests\n");
}

static int record_tests() {
    DIR *dir = opendir("./tests/");
    if(!dir) {
        fprintf(stderr, "Could not open the directory \"./tests/\".\n");
        return 1;
    }
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        if(ENDSWITH(entry->d_name, ".lox")) {
            const char* file = entry->d_name;
            char command[200] = {0};
            snprintf(command, sizeof(command), "./clox -c ./tests/%s", file);
            // printf("%s\n", command);
            if(system(command)) {
                fprintf(stderr, "COMPILATION ERROR: %s\n", file);
                return 1;
            }
            memset(command, 0, 200);
            snprintf(command, sizeof(command), "./clox -r ./tests/%s > ./tests/%s.run.output.txt", file, file);
            if(system(command)) {
                fprintf(stderr, "INTERPRETATION ERROR: %s\n", file);
                return 2;
            }

            memset(command, 0, 200);
            snprintf(command, sizeof(command), "fasm ./tests/%.*s.fasm > /dev/null", (int)(strlen(file) - strlen(".lox")), file);
            // printf("%s\n", command);
            if(system(command)) {
                fprintf(stderr, "FASM ERROR: %s\n", file);
                return 3;
            }
            memset(command, 0, 200);
            snprintf(command, sizeof(command), "./tests/%.*s > ./tests/%s.compile.output.txt", (int)(strlen(file) - strlen(".lox")), file, file);
            // printf("%s\n", command);
            if(system(command)) {
                fprintf(stderr, "RUNTIME ERROR\n");
                return 4;
            }
            char p1[200] = {0};
            char p2[200] = {0};
            snprintf(p1, sizeof(p1),  "./tests/%s.run.output.txt", file);
            snprintf(p2, sizeof(p2),  "./tests/%s.compile.output.txt", file);

            char *f1 = read_file(p1);
            char *f2 = read_file(p2);
            if(strcmp(f1, f2)) {
                fprintf(stderr, "Compiler output and interpreter output are different:\n");
                fprintf(stderr, "%s:\n%s\n\n", p1, f1);
                fprintf(stderr, "%s:\n%s\n\n", p2, f2);
                return 5;
            }
        }
    }
    closedir(dir);
    return 0;
}

static int run_tests() {
    DIR *dir = opendir("./tests/");
    if(!dir) {
        fprintf(stderr, "Could not open the directory \"./tests/\".\n");
        return 1;
    }
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        if(ENDSWITH(entry->d_name, ".lox")) {

        }
    }
    closedir(dir);
    return 0;
}

int main(int argc, char **argv) {
    switch(argc) {
        case 1:
            usage();
            return 1;
        case 2:
            if(!strcmp(argv[1], "-r") || strcmp(argv[1], "rec")) {
                return record_tests();
            }
            if(!strcmp(argv[1], "-t") || strcmp(argv[1], "test")) {
                return run_tests();
            }
            usage();
            return 1;
        default:
            usage();
            return 1;
    }
}