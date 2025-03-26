#include <stdio.h>

const int y = 10;

void func() {
    // const int x = 10;  // `x` is stored in the stack
    // int b = y;
    // printf("Address of x: %p\n", (void*)&y);
}

int main() {
    func();
    static const int x = 10;
    char *p = (char *)&x;
    *p = 20;

    printf("y: %d\n", y);
    return 0;
}
