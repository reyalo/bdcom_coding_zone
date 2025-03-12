#include<stdio.h>
#include<string.h>

typedef struct abc2{
    unsigned short a:8;
    unsigned short b:5;
    unsigned short c:4;

} test2; // 

int main(){
    test2 sta;
    memset(&sta, 0, sizeof(sta));

    sta.a = 127;
    sta.b = 15;
    sta.c = 6;      // output :


    char *p = (char *)&sta;
    // p++;

    printf("%d\n", sizeof(sta));

    printf("%x\n", *p);
    p++;
    printf("%x\n", *p);
    // Print("%x\n", *p);

    // Print("%x \n", sta);
    printf("%x \n", sta);
    




}