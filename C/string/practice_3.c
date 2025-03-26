#include <stdio.h>

void my_strcpy(char *dest, size_t dest_size, const char *src) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return; // Avoid null pointer issues or empty destination
    }

    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    dest[i] = '\0'; // Ensure null termination
}

int main() {
    char dest[6];  // Small buffer to test safety
    const char *src = "Hello World!"; // Longer than dest size

    my_strcpy(dest, sizeof(dest), src);

    printf("\n\nCopied String: \"%s\"\n", dest); // Expected: "Hello"
    
    return 0;
}
