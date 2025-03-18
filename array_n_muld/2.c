#include <stdio.h>


int main()
{
    int arr[3][2][2] = {
        {
            {1, 2},
            {3, 4}
        },
        {
            {1, 2, 3}
        },
        {
            {5, 6},
            {7, 8}
        }

    };

    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 2; ++j){
            for (int k = 0; k < 2; ++k){
                printf("%d ", arr[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}


/*

int arr[3][2][2] = {1, 2, 3, 4, 5, 6};

1 2 
3 4 

5 6 
0 0 

0 0 
0 0 



2. 
    int arr[3][2][2] = {
        {
            {1, 2},
            {3, 4}
        },
        {
            {1, 2, 3}   // invalid 
        },
        {
            {5, 6},
            {7, 8}
        }

    };


    output : (if 3 is not add)

    1 2 
    3 4 

    1 2 
    0 0 

    5 6 
    7 8 

*/