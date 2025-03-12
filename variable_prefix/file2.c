#include <stdio.h>

extern int globalVar; // Declaring (not defining)

int main() {
    printf("globalVar = %d\n", globalVar);
    return 0;
}
