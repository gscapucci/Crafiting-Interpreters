#include <stdio.h>
#include <setjmp.h>

jmp_buf buff;

void fail() {
    longjmp(buff, 1);
}

int main() {
    int i = setjmp(buff);
    printf("%d\n", i);
    if(i == 1) {
        return 1;
    }
    fail();
    return 0;
}