#include<stdio.h>
#define U_INT32_1 unsigned int*
typedef unsigned int* U_INT32_2;

#define MIN(a, b, c) (((a) < (b)) ? (((a) < (c)) ? (a) : (c)) : (((b)  < (c))? (b) : (c)))


int  main(){
    // U_INT32_1 a, b;
    unsigned int *a, *b;
    U_INT32_2 c, d;
    printf("%d\n", sizeof(a));
    printf("%d\n", sizeof(b));
    printf("%d\n", sizeof(c));
    printf("%d\n", sizeof(d));

    printf("%d\n", MIN(5, 4, 3));
    
}
