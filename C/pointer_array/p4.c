#include<stdio.h>

int main(void){

    int array[5] = {1, 2, 3, 4, 5};
    int *p = array;
    printf("%p %p\n", &p, p);           // 0xbf84b118 0xbf84b11c
    printf("%p %p\n", &array, array);   // 0xbf84b11c 0xbf84b11c

    return 0;
}