#include "scanner.h"

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

    return scan;
}


void delete_scanner(Scanner scan) {
    if(scan.source) {
        free(scan.source);
        scan.source = NULL;
    }
    delete_token_vec(scan.tokens);
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
            error(scan->line, "Unexpected character.");
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
    
    int len = scan->current - scan->start;
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

    
    uint64_t start = scan->start + 1;
    uint64_t len = scan->current - start - 1;

    char *value = malloc(len + 1);
    memcpy(value, &scan->source[start], len);
    value[len] = '\0';

    printf("%s\n", value);fflush(stdout);

    Object obj = {0};
    strcpy(obj.string, value);
    obj.type = ObjectTypeString;
    token_vec_push(&scan->tokens, create_token(STRING, value, obj, scan->line));
    
}