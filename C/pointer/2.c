#include<stdio.h>


int main(void)
{
    int a = 1;
    int *p = &a;
    *p = 2;
    p = 1;
    // *P = 2;
    return 0;
}
