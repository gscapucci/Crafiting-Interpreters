HEADER_FILES=	macro.h lox.h error.h macro.h scanner.h\
				token_type.h token_vec.h token.h hashmap.h\
				expr.h parser.h astprinter.h interpreter.h

SOURCE_FILES=	main.c lox.c error.c scanner.c token_vec.c\
				token.c token_type.c hashmap.c expr.c parser.c\
				astprinter.c interpreter.c
FILES=$(SOURCE_FILES) $(HEADER_FILES)
CC=gcc
FLAGS=-Wall -Wextra
LINK=-lm
OUTPUT=clox

LOX_INPUT=input.lox
VALGRIND=valgrind
VALGRIND_FLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
A_OUT=a.out

main: $(FILES)
	$(CC) $(FLAGS) -O3 $(SOURCE_FILES) -o $(OUTPUT) $(LINK)

memcheck: $(FILES)
	$(CC) $(FLAGS) -g $(SOURCE_FILES) $(LINK)
	$(VALGRIND) $(VALGRIND_FLAGS) ./$(A_OUT) $(LOX_INPUT)