#include<stdio.h>

// static int x = 10;

void fn(void)
{
    static int x = 10;
    printf("%d\n", x);
    ++x;
}

int main(){
    // printf("%d\n", x);
    fn();
    fn();
    fn();
    // printf("%d\n", x);
}