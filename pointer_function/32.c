#include <stdio.h>
#include<string.h>
#include<stdlib.h>  

char *get_memory (void)
{
    char p[] = "Hello World!";
    return p;
}

int main(void)
{
    char *str = NULL;
    str = get_memory();
    printf("%s\n", str);
    // printf(str);
    return 0;
}