#include "scanner.h"
#include "error.h"
#include "hashmap.h"

Scanner create_scanner(const char *source) {
    Scanner scan;
    scan.start = 0;
    scan.current = 0;
    scan.line = 1;
    scan.source = source;
    scan.vec = create_token_vec();
    scan.keywords = create_hashmap(16, hash_string, malloc, free, compare_string, hashmap_copy_tokentype, hashmap_copy_string, FLAG_COPY_VALUE);
    hashmap_insert(&scan.keywords, "and", &(enum TokenType){AND});
    hashmap_insert(&scan.keywords, "class", &(enum TokenType){CLASS});
    hashmap_insert(&scan.keywords, "else", &(enum TokenType){ELSE});
    hashmap_insert(&scan.keywords, "false", &(enum TokenType){FALSE});
    hashmap_insert(&scan.keywords, "for", &(enum TokenType){FOR});
    hashmap_insert(&scan.keywords, "fun", &(enum TokenType){FUN});
    hashmap_insert(&scan.keywords, "if", &(enum TokenType){IF});
    hashmap_insert(&scan.keywords, "nil", &(enum TokenType){NIL});
    hashmap_insert(&scan.keywords, "or", &(enum TokenType){OR});
    hashmap_insert(&scan.keywords, "print", &(enum TokenType){PRINT});
    hashmap_insert(&scan.keywords, "return", &(enum TokenType){RETURN});
    hashmap_insert(&scan.keywords, "super", &(enum TokenType){SUPER});
    hashmap_insert(&scan.keywords, "this", &(enum TokenType){THIS});
    hashmap_insert(&scan.keywords, "true", &(enum TokenType){TRUE});
    hashmap_insert(&scan.keywords, "var", &(enum TokenType){VAR});
    hashmap_insert(&scan.keywords, "while", &(enum TokenType){WHILE});
    return scan;
}

TokenVec scan_tokens(Lox *lox, Scanner *scan) {
    while(!scan_is_at_end(scan)) {
        scan->start = scan->current;
        scan_token(lox, scan);
    }

    token_vec_push(&scan->vec, create_token(EEOF, "", OBJECT_NONE(), scan->line));
    return scan->vec;
}

void delete_scanner(Scanner *scan) {
    if(!scan) return;
    delete_hashmap(&scan->keywords);
}

bool scan_is_at_end(Scanner *scan) {
    return scan->current >= strlen(scan->source);
}

void scan_token(Lox *lox, Scanner *scan) {
    char c = scan_advance(scan);

    switch(c) {
        case '(': add_token(scan, LEFT_PAREN); break;
        case ')': add_token(scan, RIGHT_PAREN); break;
        case '{': add_token(scan, LEFT_BRACE); break;
        case '}': add_token(scan, RIGHT_BRACE); break;
        case ',': add_token(scan, COMMA); break;
        case '.': add_token(scan, DOT); break;
        case '-': add_token(scan, MINUS); break;
        case '+': add_token(scan, PLUS); break;
        case ';': add_token(scan, SEMICOLON); break;
        case '*': add_token(scan, STAR); break;
        case '!': add_token(scan, scan_match(scan, '=') ? BANG_EQUAL : BANG); break;
        case '=': add_token(scan, scan_match(scan, '=') ? EQUAL_EQUAL : EQUAL); break;
        case '<': add_token(scan, scan_match(scan, '=') ? LESS_EQUAL : LESS); break;
        case '>': add_token(scan, scan_match(scan, '=') ? GREATER_EQUAL : GREATER); break;
        case '/':
            if(scan_match(scan, '/')) {
                while(scan_peek(scan) != '\n' && !scan_is_at_end(scan)) scan_advance(scan);
            } else {
                add_token(scan, SLASH);
            }
            break;
        case ' ':
            break;
        case '\r':
            break;
        case '\t':
            break;
        case '\n':
            scan->line++;
            break;
        case '\"':
            string(lox, scan);
            break;
        default:
            if(is_digit(c)) {
                number(scan);
            } else if(is_alpha(c)) {
                identifier(scan);
            } else {
                error(lox, scan->line, "Unexpect character.");
            }
            break;
    }
}

void identifier(Scanner *scan) {
    while(is_alpha_numeric(scan_peek(scan))) scan_advance(scan);
    char *text = calloc(scan->current - scan->start + 1, 1);
    memcpy(text, &scan->source[scan->start], scan->current - scan->start);
    enum TokenType *type = hashmap_get(&scan->keywords, text);
    if(type == NULL) {
        type = malloc(sizeof(enum TokenType));
        *type = IDENTIFIER;
    }
    add_token(scan, *type);
    if(*type == IDENTIFIER) {
        free(type);
    }
    free(text);
}

bool is_alpha(const char c) {
    return  (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
             c == '_';
}

bool is_alpha_numeric(const char c) {
    return is_alpha(c) || is_digit(c);
}

void number(Scanner *scan) {
    while(is_digit(scan_peek(scan))) scan_advance(scan);
    bool is_float = false;
    if(scan_peek(scan) == '.' && is_digit(peek_next(scan))) {
        is_float = true;
        scan_advance(scan);
        while(is_digit(scan_peek(scan))) scan_advance(scan);
    }
    char *substr = calloc(scan->current - scan->start + 1, 1);
    memcpy(substr, scan->source + scan->start, scan->current - scan->start);
    if(is_float) {
        add_token_obj(scan, FLOAT, create_object_from_double(strtod(substr, NULL)));
    } else {
        add_token_obj(scan, INT, create_object_from_int(strtoll(substr, NULL, 10)));
    }
    free(substr);
}

char peek_next(Scanner *scan) {
    if(scan->current + 1 >= strlen(scan->source)) return '\0';
    return scan->source[scan->current + 1];
}

bool is_digit(const char c) {
    return c >= '0' && c <= '9';
}

void string(Lox *lox, Scanner *scan) {
    while(scan_peek(scan) != '\"' && !scan_is_at_end(scan)) {
        if(scan_peek(scan) == '\n') scan->line++;
        scan_advance(scan);
    }
    if(scan_is_at_end(scan)) {
        error(lox, scan->line, "Unterminated string.");
        return;
    }
    scan_advance(scan);

    char *value = calloc((scan->current - 1) - (scan->start + 1) + 1, 1);
    memcpy(value, &scan->source[scan->start + 1], (scan->current - 1) - (scan->start + 1));
    Object obj = create_object_from_str(value);
    add_token_obj(scan, STRING, obj);
    delete_object(&obj);
    free(value);
}

char scan_peek(Scanner *scan) {
    if(scan_is_at_end(scan)) return '\0';
    return scan->source[scan->current];
}

bool scan_match(Scanner *scan, const char expected) {
    if(scan_is_at_end(scan)) return false;
    if(scan->source[scan->current] != expected) return false;
    scan->current++;
    return true;
}

char scan_advance(Scanner *scan) {
    return scan->source[scan->current++];
}

void add_token(Scanner *scan, const enum TokenType type) {
    add_token_obj(scan, type, OBJECT_NONE());
}

void add_token_obj(Scanner *scan, const enum TokenType type, const Object literal) {
    char *text = calloc(scan->current - scan->start + 1, 1);
    memcpy(text, scan->source + scan->start, scan->current - scan->start);
    token_vec_push(&scan->vec, create_token(type, text, literal, scan->line));
    free(text);
}
