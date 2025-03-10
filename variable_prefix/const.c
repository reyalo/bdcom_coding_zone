#include <stdio.h>

const int yy = 10;                 // y is stored in .ro data section
static const int z = 10;           // z is stored in .ro data section



int main() {

    static const int x = 10;       // x is stored in .ro data section

    const int y = 10;              // y is stored in the stack frame
    char *p = (char *)&y;

    *p = 20;

    printf("y: %d\n", y);
    return 0;
}
