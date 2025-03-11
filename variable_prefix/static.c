#include <stdio.h>

static int count_global = 0;        // though it is global, it can be accessed only in this file

void counter() {
    static int count = 0;           // Stored in `.data`, not reinitialized
    count++;
    printf("Count: %d\n", count);   // 1, 2, 3
}

int main() {
    counter();
    counter();
    counter();

    // printf("Count: %d\n", count);

    return 0;
}
