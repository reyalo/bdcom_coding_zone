#include<stdio.h>



int main(){

    char *str = NULL;

    char arr[] = "How are you?";

    str = &arr;

    char **p;
    p = &str;

    printf("str: %s\n", str);

}