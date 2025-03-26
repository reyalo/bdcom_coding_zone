#include<stdio.h>

int main(void)
{

    int array[] = {1, 2, 3};
    printf("%p %p\n", array, array + 1);        // exbfec79f4, exbfec79f8

    printf("%p %p\n", &array[0], &array[0] + 1);// exbfec79f4, exbfec79f8

    printf("%p %p\n", &array, &array + 1);      // exbfec79f4, 0xbfec7a00

    return 0;
}


