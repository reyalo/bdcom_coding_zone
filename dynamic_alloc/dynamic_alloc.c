#include<stdio.h>
#include<stdlib.h>
#include "../custom.h"

int main(){

    char *p = (char *) malloc(10);

    *p = 10;

    
    char *pr = (char *) realloc(p, 12);
    
    print_char(p, 10);

    char *p1 = (char *) calloc(2, 4);

    print_char(pr, 20);

    // printf("\n");

    print_char(p1, 50);

    

}