#include <stdio.h>
#include <string.h>

// Reentrant: no global/static state
void safe_function(const char* input, char* output) {
    strcpy(output, input);  // Uses caller's buffer
}

int main() {
    char a[100], b[100];

    safe_function("Hello", a);
    safe_function("World", b);

    printf("a: %s\n", a);  // "Hello"
    printf("b: %s\n", b);  // "World"
    return 0;
}
