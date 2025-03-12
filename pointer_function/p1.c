
#include <stdio.h>

void test_fun(int a, void* b, char *c)
{
    *(int*) a = 11;
    // printf("%x \n", a);
    *(int*) b = 22;
    *(int*) c = 33;
    return;
}

int main(void)
{
    int a = 1, b = 2, c = 3;
    test_fun (&a, &b, &c);
    printf("a=%d, b=%d, c=%d\n", a, b, c);   // 11 22 33
    return 0;
    
}