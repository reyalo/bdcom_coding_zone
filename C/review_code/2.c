#include<stdio.h>
#include<string.h>

typedef struct abc2{
    unsigned char a:2;
    unsigned char b:3;
    unsigned char c:4;

} test2; // 

int main(){
    test2 sta;
    memset(&sta, 0, sizeof(sta));

    sta.a = 2;
    sta.b = 3;
    sta.c = 6;      // output :


    unsigned char *p = (unsigned char *)&sta;
    printf("%x\n", *p);
    p++;
    printf("%x\n", *p);
    p++;
    printf("%x\n", *p);
    // Print("%x\n", *p);

    // Print("%x \n", sta);
    // printf("%x\n", sta);
    printf("%x \n", *((unsigned short *)&sta));
    




}