#include <stdio.h>
const int x = 10;
int main() {
    
    int *ptr = (int *)&x;  // Cast away constness
    *ptr = 20;  // Modify x

    printf("x = %d\n", x);  // Undefined behavior (may or may not change)
    return 0;
}
