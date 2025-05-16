#include<stdio.h>

typedef struct abc{
    unsigned short a:2;
    unsigned short b:3;
    unsigned short c:2;
    unsigned short d:4;

} test;

int main(){
    test sta;

    sta.a = 6;
    sta.b = 6;
    sta.c = 6;
    sta.d = 6;


    printf("%x \n", sta);
    




}