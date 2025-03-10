#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void get_memory (char **p, int num)
{
    if (p == NULL)
    return;
    *p = (char *)malloc(num);
    return;
}

int main(void)
{
    char *str = NULL;
    printf("In main %d\n", str);
    get_memory(&str, 100);

    strcpy(str, "Hello World!");
    printf(str);
    return 0;
}