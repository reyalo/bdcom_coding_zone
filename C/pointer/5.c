#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr = (int*)malloc(sizeof(int)); // Allocate memory
    *ptr = 42;
    
    free(ptr); // Free memory, but ptr still holds the old address (dangling)

    printf("\n%d\n", *ptr); // Undefined behavior (dangling pointer access)

    return 0;
}
