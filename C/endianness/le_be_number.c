#include<stdio.h>

int main(){
    int a = 0x21fedcba;

    unsigned char *p = &a;
    printf("%x\n", p[0]);
}