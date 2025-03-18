#include <stdio.h>


int main() {
    
    int arr[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};

    printf("%d\n", arr[0][0]);

    // int *ptr = &arr;

    printf("%d\n", *(*(arr)));


    // array_____debug

    printf("%p\n", arr);                    // 0x0000        // single time indirection of 2d array give address of a row
    printf("%p\n", *(arr+1));               // 0x0010        // single time indirection of 2d array give address of a row
    printf("%p\n", *(arr+2));               // 0x0020       // single time indirection of 2d array give address of a row
    printf("%d\n", *(*(arr+2) + 0));        // 9            // double time indirection of 2d array give value of a cell

    
    return 0;
}