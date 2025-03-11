#include <stdio.h>

int main(void)
{
    int array1[3][6] = {
        1, 2, 3, 4, 5, 6,
        7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18
    };
    int array2[2][3][4] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,

        13, 14, 15, 16,
        17, 18, 19, 20,
        21, 22, 23, 24
    };

    printf("%d %d\n", array1[1][6], array1[0][15]);  //
    printf("%d %d\n", array2[0][3][4], array2[0][2][8]);  // Undefined behavior (out-of-bounds access)

    return 0;
}
