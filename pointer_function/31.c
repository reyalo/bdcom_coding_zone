#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void get_memory (char *p)
{
    p = (char *)malloc(100);
    return;
}
int main(void)
{
    char *str = NULL;
    get_memory(str);

    //str = (char *)malloc(100);
    strcpy(str, "Hello World!");
    printf(str);
    return 0;
}