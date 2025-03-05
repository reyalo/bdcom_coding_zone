#include <stdio.h>

void my_strncat(char *dest, char *src, int dest_len) {
    if (dest == NULL || src == NULL || dest_len <= 0) {
        return; // Prevent invalid input
    }

    int dest_current_len = 0;
    
    // Find the length of dest (but within bounds)
    while ( (dest_current_len < dest_len - 1) && (dest[dest_current_len] != '\0')) {
        dest_current_len++;
    }

    // Start appending src at the end of dest
    int i = 0;
    while (dest_current_len < dest_len - 1 && src[i] != '\0') {
        dest[dest_current_len] = src[i];
        dest_current_len++;
        i++;
    }

    // Null terminate the string
    dest[dest_current_len] = '\0';
}

int main() {
    char dest[12] = "Hello";  // 12 bytes allocated, "Hello" takes 6 (5 chars + '\0')
    char src[] = " World!!!";

    my_strncat(dest, src, sizeof(dest));

    printf("\n\nConcatenated String: \"%s\"\n", dest); // Expected: "Hello World"
    
    return 0;
}
