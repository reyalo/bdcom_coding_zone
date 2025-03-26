#include<stdio.h>

int main(void)
 {

    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    int *p = a;
    int *q = &a + 1;

    int m = 22;
    int n = 33;

    p += 3;

    printf("%d %d\n", *p, q- p);    // 4 6
    printf("%d\n", &m - &n);        // 1

    return 0;
 }

