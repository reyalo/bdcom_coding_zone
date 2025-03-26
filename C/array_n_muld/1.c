#include <stdio.h>
#include <stdlib.h>

// argument of a function
void processMatrix(int (*arr)[4], int rows) {
    // Can use array notation
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < 4; j++) {
            arr[i][j] *= 2;  // Direct array access
        }
    }
}

int main() {
    int array_2d[2][3] = { {1, 2, 3}, {4, 5, 6} };
    int (*ptr)[3] = array_2d;               // Pointer to an array of 3 integers


    
    for (int i = 0; i < 2; i++) {           // Access each element using pointer arithmetic
        for (int j = 0; j < 3; j++) {
            printf("%d ", ptr[i][j]);       // Equivalent to array_2d[i][j]
        }
        printf("\n");
    }

    // Clean Multi-dimensional Array Processing
    int matrix[3][4];
    int (*ptr)[4] = matrix;


    // Allocate 2D array dynamically
    int rows = 3, cols = 4;
    int (*dynamic)[4] = malloc(rows * sizeof(*dynamic));

    // Use and free
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            dynamic[i][j] = 2;
        }
    }

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%d ", dynamic[i][j]);
        }
        printf("\n");
    }

    free(dynamic);

    return 0;
}
