#include <stdio.h>

int main() {
    char *arr[] = {"Hello", "World", "C", NULL};  // Array of pointers to strings.
    char **ptr = arr;                             // Pointer to pointer pointing 
                                                  // to the first element.

    while (*ptr) {
        printf("%s\n", *ptr);
        ptr++;                                    // Move to the next pointer.
    }

    return 0;
}
