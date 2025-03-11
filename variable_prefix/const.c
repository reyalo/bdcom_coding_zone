#include <stdio.h>

// for const variables,
// best pracice is variable_type const variable_name
// e.g. int const *x = &a;
// e.g. int *const x = &a;
// e.g. int const *const x = &a;    // constant pointer to constant integer

const int yy = 10;                 // y is stored in .ro data section
static const int z = 10;           // z is stored in .ro data section



int main() {

    static const int x = 10;       // x is stored in .ro data section

    const int y = 10;              // y is stored in the stack frame
    char *p = (char *)&y;

    *p = 20;

    printf("y: %d\n", y);

    // int xx = 10;

    // *const int ptr ;
    // int *const ptr1 = &xx;

    // ptr = &x;
    // ptr1 = &x;
    
    // *ptr = 20;
    // *ptr1 = 20;  
    


    return 0;
}
