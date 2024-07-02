INPUTFILES=src/main.c src/lox.c src/token.c src/error.c src/scanner.c src/object.c src/token_type.c src/token_vec.c src/hashmap.c
HEADER_FILES=include/lox.h include/error.h include/lox.h include/scanner.h include/token_type.h include/token.h include/object.h include/token_vec.h include/token.h include/hashmap.h include/expr.h

ALLFILES= $(INPUTFILES) $(HEADER_FILES)
FLAGS=-Wall -Wextra -Wpedantic
LINK=
OUTPUT=bin/lox
OUTPUT_DBG=bin/a.out


lox: $(ALLFILES)
	if [ -d "bin/" ]; then\
		gcc $(FLAGS) $(INPUTFILES) -o $(OUTPUT) $(LINK);\
		gcc -g $(INPUTFILES) $(LINK) -o $(OUTPUT_DBG);\
	else\
		mkdir bin;\
		gcc $(FLAGS) $(INPUTFILES) -o $(OUTPUT) $(LINK);\
		gcc -g $(INPUTFILES) $(LINK) -o $(OUTPUT_DBG);\
	fi


memcheck:
	make
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(OUTPUT_DBG)