#include<stdio.h>

void func(int *p)
{
    *p += 1;
    return;
}

int main(void)
{
    int a = 2;
    func(&a);
    printf("%d\n", a);
    return 0;

}