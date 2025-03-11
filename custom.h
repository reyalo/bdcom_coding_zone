#include<stdio.h>
void prp(int sz, unsigned char *p){

    p = p+8;
    for(int i=0; i < sz ; ++i){
        printf("%x ", *--p);
    }
    printf("\n");

}