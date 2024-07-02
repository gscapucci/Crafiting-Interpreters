#include "../include/scanner.h"

Scanner create_scanner(const char *source) {
    Scanner scan = {0};
    scan.tokens = create_token_vec(20);
    scan.current = 0;
    scan.start = 0;
    scan.line = 1;

    uint64_t strlen_source = strlen(source);
    scan.source = malloc(strlen_source + 1);
    strcpy(scan.source, source);
    scan.source[strlen_source] = 0;

    scan.keywords = create_hashmap();

    hashmap_insert(&scan.keywords, "and",    AND);
    hashmap_insert(&scan.keywords, "class",  CLASS);
    hashmap_insert(&scan.keywords, "else",   ELSE);
    hashmap_insert(&scan.keywords, "false",  FALSE);
    hashmap_insert(&scan.keywords, "for",    FOR);
    hashmap_insert(&scan.keywords, "fun",    FUN);
    hashmap_insert(&scan.keywords, "if",     IF);
    hashmap_insert(&scan.keywords, "nil",    NIL);
    hashmap_insert(&scan.keywords, "or",     OR);
    hashmap_insert(&scan.keywords, "print",  PRINT);
    hashmap_insert(&scan.keywords, "return", RETURN);
    hashmap_insert(&scan.keywords, "super",  SUPER);
    hashmap_insert(&scan.keywords, "this",   THIS);
    hashmap_insert(&scan.keywords, "true",   TRUE);
    hashmap_insert(&scan.keywords, "var",    VAR);
    hashmap_insert(&scan.keywords, "while",  WHILE);

    return scan;
}


void delete_scanner(Scanner scan) {
    if(scan.source) {
        free(scan.source);
        scan.source = NULL;
    }
    delete_token_vec(scan.tokens);
    delete_hashmap(&scan.keywords);
}

TokenVec scan_tokens(Scanner *scan) {
    while(!is_at_end(scan)) {
        scan->start = scan->current;
        scan_token(scan);
    }
    token_vec_push(&scan->tokens, create_token(EOFF, "", (Object){.type = ObjectNone}, scan->line));
    
    return scan->tokens; //moved
}


bool is_at_end(Scanner *scan) {
    return scan->current >= strlen(scan->source);
}

void scan_token(Scanner *scan) {
    char c = advance(scan);
    switch (c) {
        case '(':add_token(scan, LEFT_PAREN);break;
        case ')':add_token(scan, RIGHT_PAREN);break;
        case '{':add_token(scan, LEFT_BRACE);break;
        case '}':add_token(scan, RIGHT_BRACE);break;
        case ',':add_token(scan, COMMA);break;
        case '.':add_token(scan, DOT);break;
        case '-':add_token(scan, MINUS);break;
        case '+':add_token(scan, PLUS);break;
        case ';':add_token(scan, SEMICOLON);break;
        case '*':add_token(scan, STAR);break;
        case '!':
            add_token(scan, match(scan, '=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            add_token(scan, match(scan, '=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            add_token(scan, match(scan, '=') ? LESS_EQUAL : LESS);
            break;
        case '>':
            add_token(scan, match(scan, '=') ? GREATER_EQUAL : GREATER);
            break;
        case '/':
            if(match(scan, '/')) {
                while(peek(scan) != '\n' && !is_at_end(scan)) advance(scan);
            } else {
                add_token(scan, SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            scan->line++;
            break;
        case '"':
            string(scan);
            break;
        default:
            if(is_digit(c)) {
                number(scan);
            } else if (is_alpha(c)) {
                identifier(scan);
            } else {
                error(scan->line, "Unexpected character.");
            }
            break;
    }
}

char advance(Scanner *scan) {
    return scan->source[scan->current++];
}

void add_token(Scanner *scan, enum TokenType type) {
    add_token_obj(scan, type, (Object){.type = ObjectNone});
}

void add_token_obj(Scanner *scan, enum TokenType type, Object literal) {
    
    uint64_t len = scan->current - scan->start;
    char *text = malloc(len + 1);
    memcpy(text, &scan->source[scan->start], len);
    text[len] = 0;
    token_vec_push(&scan->tokens, create_token(type, text, literal, scan->line));
    free(text);
}

bool match(Scanner *scan, char expected) {
    if(is_at_end(scan)) return false;
    if(scan->source[scan->current] != expected) return false;
    scan->current++;
    return true;
}

char peek(Scanner *scan) {
    if(is_at_end(scan)) return '\0';
    return scan->source[scan->current];
}

void string(Scanner *scan) {
    while (peek(scan) != '"' && !is_at_end(scan)) {
        if(peek(scan) == '\n') scan->line++;
        advance(scan);
    }
    if(is_at_end(scan)) {
        error(scan->line, "Unterminated string.");
        return;
    }

    advance(scan);

    Object obj = {0};
    obj.type = ObjectTypeString;
    
    uint64_t start = scan->start + 1;
    uint64_t len = scan->current - start - 1;

    memcpy(obj.string, &scan->source[start], len);

    token_vec_push(&scan->tokens, create_token(STRING, obj.string, obj, scan->line));
    
}

void number(Scanner *scan) {
    while(is_digit(peek(scan))) advance(scan);

    if(peek(scan) == '.' && is_digit(peek_next(scan))) {
        advance(scan);
        while(is_digit(peek(scan))) advance(scan);
    }

    Object obj = {0};
    obj.type = ObjectTypeNum;

    uint64_t len = scan->current - scan->start;
    
    char *sub_str = malloc(len + 1);
    memcpy(sub_str, scan->source + scan->start, len);
    sub_str[len] = '\0';

    obj.num = strtod(sub_str, NULL);

    token_vec_push(&scan->tokens, create_token(NUMBER, sub_str, obj, scan->line));

    free(sub_str);
}

char peek_next(Scanner *scan) {
    if(scan->current + 1 >= strlen(scan->source)) return '\0';
    return scan->source[scan->current + 1];
}

void identifier(Scanner *scan) {
    while(is_alphanumeric(peek(scan))) advance(scan);

    Object obj = {0};
    obj.type = ObjectNone;

    uint64_t len = scan->current - scan->start;
    char *sub_str = malloc(len + 1);
    memcpy(sub_str, scan->source + scan->start, len);
    sub_str[len] = '\0';

    enum TokenType type = hashmap_find(&scan->keywords, sub_str);
    if(type == -1) {
        type = IDENTIFIER;
    }

    token_vec_push(&scan->tokens, create_token(type, sub_str, obj, scan->line));
    free(sub_str);
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}
bool is_alphanumeric(char c) {
    return is_alpha(c) || is_digit(c);
}