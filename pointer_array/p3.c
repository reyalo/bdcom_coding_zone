#include<stdio.h>

void fun(int array[])
{
    printf("The lengh of array in fun is %d\n", sizeof(array)); // 8 for 64 bits os
    return;
}

int main(void)
{

    int array[5] = {1, 2, 3, 4, 5};
    fun(array);
    printf("The lengh of array in main is %d\n", sizeof(array));    // 20
    return 0;
}