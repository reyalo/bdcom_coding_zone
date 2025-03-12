#include <stdio.h>

void counter() {
    static int count = 0;  // Stored in the data segment (not stack)
    count++;               // Value is preserved between function calls
    printf("Count: %d\n", count);
}

int main() {
    counter();
    counter();
    counter();
    return 0;
}
