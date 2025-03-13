#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int a = 2, *p = &a, **q= &p;
    int **p1 = &a;
    int *p2 = &p;
    printf("%d %d\n",*p1, **(int **)p2);  // 2 2


    // printf("The value of a is 0x%-8x, Its address is %p\n", a, &a);
    // printf("The value of p is 0x%-8x, Its address is %p\n", p, &p);
    // printf("The value of q is 0x%-8x, Its address is %p\n", q, &q);

    // *a, **a
    // *p, **p
    // *q, **q
    // printf("The value of *a = %d and **a = %d\n", *a, **a);
    // printf("The value of *p = %d and **p = %d\n", *p, **p);
    // printf("The value of *q = %d and **q = %d\n", *q, **q);


    return 0;
}