#include <stdio.h>

int main() {
    int *ptr;  // Wild pointer (not initialized)

    *ptr = 10; // Undefined behavior. It points to a random memory location
    printf("%d\n", *ptr);

    return 0;
}
