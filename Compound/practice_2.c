#include <stdio.h>

int main() {
    int x = 100; // Stored next to arr in memory
    int arr[7] = {1, 2, 3, 4, 5};

    printf("\n\nBefore overflow: x = %d\n", x);

    // Writing beyond the allocated memory of arr
    arr[7] = 999;  // Overflow: arr only has indexes 0 to 6!

    printf("After overflow: x = %d\n\n", x); // Unexpected behavior

    int array[5] = {1, 2, 3, 4, 5};

    // Address of arr[i] = Base Address + (i * sizeof(type))
    // for arr[2];
    // Address = 1000 + (2 * 4) = 1008

    //   *(arr + i)  // Pointer arithmetic




    return 0;
}
