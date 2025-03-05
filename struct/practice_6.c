// testing big endian and Little edian
#include<stdio.h>
#include<string.h>
struct abc{
    unsigned char a:2;
    unsigned char b:3;
    unsigned char c:3;
};

int main(){

    

    // code for my selft testing
    int a = 513;
    char *p = (char *)&a;
    unsigned c0 = *p++;
    unsigned c1 = *p++;
    unsigned c2 = *p++;
    unsigned c3 = *p++;
    printf("\n\nOutput in little Endian machine will be \"01 02 00 00\"\n\n");
    printf("Output in Big Endian machine will be    \"00 00 02 01\"\n\n");
    printf("The Byte Order in this machine low -> high = %02x %02x %02x %02x \n\n", c0, c1, c2, c3);


    // code of practice_6
    unsigned char aa;
    struct abc v;
    v.a = 2;
    v.b=3;
    v.c=6;
    // v.c=0;
    memcpy(&aa, &v, sizeof(v));
    printf("\n%x\n",aa);
    

}