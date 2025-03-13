#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int a = 20;
    int *p = &a;

    // there is no difference in %x and %p.
    printf(" %x  %p \n", a, a); // output: 14 00000000000014
}