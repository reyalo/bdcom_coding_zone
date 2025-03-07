#include <stdio.h>

int main() {
    int array[5];       // Reserve space for 5 integers
    int *ptr;           // Pointer to manipulate memory

                        // Initialize array elements
    array[0] = 1;       // movl $1, -24(%ebp)
    array[1] = 2;       // movl $2, -20(%ebp)
    array[2] = 3;       // movl $3, -16(%ebp)
    array[3] = 4;       // movl $4, -12(%ebp)
    array[4] = 5;       // movl $5, -8(%ebp)

    ptr = array;        // Set pointer to &array[0]
    printf("%x\n", array);
    printf("%x\n", ptr);
    ptr = ptr + 2;      // addl $8, %eax
    printf("%x\n", ptr);

                        // movl %eax, -4(%ebp)

    array[1] = 11;      // movl $11, -20(%ebp)

    *(ptr + 1) = 22;    // movl -4(%ebp), %eax
                        // addl $4, %eax
                        // movl $22, (%eax)

    array[2] = 33;      // movl $33, -16(%ebp)

    *(ptr + 2) = 44;    // movl -4(%ebp), %eax
                        // addl $8, %eax
                        // movl $44, (%eax)

    return 0;           // movl $0, %eax
}
