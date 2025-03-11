#include <stdio.h>

int main()
{
    int array[5] = {1, 2, 3, 4, 5};

    int arr[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    int x = arr[0][1];  // Accessing the element in the second row, third column 
                        //(value 2 for the previous example)



    // *(arr + i) + j  // This gives the address of arr[i][j]
    // applying this formula
    // *(*(arr + i) + j)  // Dereferencing it gives the value at arr[i][j]
    // array1[1][6]    = *((100 + 1*6*4) + 6*4) = *(148) = 13
    // array1[0][15]   = *((100 + 0*6*4) + 15*4) = *(160) = 16

    // *(*(*(arr + i) + j) + k)  // Dereferencing it gives the value at arr[i][j][k]
    // array2[0][3][4] = *(((200 + 0*3*4*4) + 3*4*4) + 4*4) = *(264) = 17
    // array2[0][3][4] = *(((200 + 0*3*4*4) + 2*4*4) + 8*4) = *(264) = 17

    // *(*(arr + i) + j)  // Dereferencing it gives the value at arr[i][j]



    // Address = Base_Address + (row * num_cols + col) * sizeof(data type)

                        
    int i, j;
    i = 1, j = 1;

    printf("%d\n", *(*(arr + i) + j));
    printf("%p\n", &array[0]);
    printf("%p\n", &array[-1]);  // Undefined behavior (out-of-bounds access)

    printf("%p\n", &array[4]);
    printf("%p\n", &array[5]);   // Undefined behavior (out-of-bounds access)

    

    return 0;
}
