HEADER_FILES= chunk.h common.h debug.h memory.h value.h vm.h compiler.h scanner.h object.h
SOURCE_FILES= chunk.c debug.c main.c memory.c value.c vm.c compiler.c scanner.c object.c
FILES=$(SOURCE_FILES) $(HEADER_FILES)
CC=gcc
FLAGS=-Wall -Wextra
LINK=-lm
OUTPUT=clox

LOX_INPUT=input.lox
VALGRIND=valgrind
VALGRIND_FLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
A_OUT=a.out

main: debug
	$(CC) -O3 $(SOURCE_FILES) -o $(OUTPUT) $(LINK)

memcheck: debug
	$(VALGRIND) $(VALGRIND_FLAGS) ./$(A_OUT) $(LOX_INPUT)

debug: $(FILES)
	$(CC) $(FLAGS) -g $(SOURCE_FILES) $(LINK)