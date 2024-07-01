INPUTFILES=main.c lox.c token.c error.c scanner.c object.c token_type.c token_vec.c
ALLFILES= $(INPUTFILES) lox.h error.h lox.h scanner.h token_type.h token.h object.h token_vec.h token.h
FLAGS=-Wall -Wextra -Wpedantic
LINK=-lm
OUTPUT=lox


lox: $(ALLFILES) 
	gcc $(FLAGS) $(INPUTFILES) -o $(OUTPUT) $(LINK)
	gcc -g $(INPUTFILES) $(LINK)


memcheck:
	make
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(OUTPUT)