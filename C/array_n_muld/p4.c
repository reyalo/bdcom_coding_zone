#include<stdio.h>
 //.
// array . .
//  array + 2 . .
//  array[3] . .
//  array[2] - 1 . .
//  array[2][1] . .
//  array[1][0] + 1 . .
//  array[1][0][2] . .
//  array[0][1][0] + 2 . .
//  array[3][1][2][5] . .
//  &array[3][1][2][5] . 

 int main(){
    int array[4][2][3][6];

    long long x = (long long)&array;
    printf("%p\n", x);
    printf("%x\n", (long long )array - x + 0x1000);
    printf("%x\n", (long long )(array+2) - x + 0x1000);
    printf("%x\n", (long long )(array[3]) - x + 0x1000);
    printf("%x\n", (long long )(array[2] - 1) - x + 0x1000);
    printf("%x\n", (long long )(array[2][1]) - x + 0x1000);
    printf("%x\n", (long long )(array[1][0] + 1) - x + 0x1000);
    printf("%x\n", (long long )(array[1][0][2]) - x + 0x1000);
    printf("%x\n", (long long )(array[0][1][0] + 2) - x + 0x1000);
    printf("%x\n", (long long )(&array[3][1][2][5]) - x + 0x1000);


 }
