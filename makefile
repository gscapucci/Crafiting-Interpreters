HEADER_FILES= 	./lox/chunk.h ./lox/common.h ./lox/debug.h ./lox/memory.h\
				./lox/value.h ./lox/vm.h ./lox/compiler.h ./lox/scanner.h\
				./lox/object.h ./lox/table.h ./lox/assembly.h

SOURCE_FILES= 	./lox/chunk.c ./lox/debug.c ./lox/main.c ./lox/memory.c\
				./lox/value.c ./lox/vm.c ./lox/compiler.c ./lox/scanner.c\
				./lox/object.c ./lox/table.c ./lox/assembly.c
FILES=$(SOURCE_FILES) $(HEADER_FILES)
CC=gcc
FLAGS=-Wall -Wextra
LINK=-lm
OUTPUT=clox

LOX_INPUT=input.lox
VALGRIND=valgrind
VALGRIND_FLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
A_OUT=a.out

TESTER_FILES=./tester/main.c
TESTER_OUTPUT=test

main: debug tester
	$(CC) -O3 $(SOURCE_FILES) -o $(OUTPUT) $(LINK)

memcheck: debug
	$(VALGRIND) $(VALGRIND_FLAGS) ./$(A_OUT) -r $(LOX_INPUT)

debug: $(FILES)
	$(CC) $(FLAGS) -g $(SOURCE_FILES) $(LINK)

tester: $(TESTER_FILES)
	$(CC) $(FLAGS) $(TESTER_FILES) -o $(TESTER_OUTPUT)