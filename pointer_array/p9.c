#include<stdio.h>

int main(){
    char *str = "Hello World!";
    char str2[13] = "Hello World";

    str[2] = 11;
    str2[2] = 22;

    return 0;
}