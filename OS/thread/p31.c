#include <stdio.h>
#include <string.h>

// Global static buffer — shared across calls!
char* unsafe_function(const char* input) {
    static char buffer[100];
    strcpy(buffer, input);
    return buffer;
}

int main() {
    char* a = unsafe_function("Hello");
    char* b = unsafe_function("World");

    printf("a: %s\n", a);  // Will print "World" (overwritten)
    printf("b: %s\n", b);  // Will print "World"
    return 0;
}
