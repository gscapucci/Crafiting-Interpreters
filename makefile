HEADER_FILES=

SOURCE_FILES=
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