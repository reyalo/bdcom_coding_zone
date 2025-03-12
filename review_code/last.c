#include<stdio.h>
#include<string.h>

typedef struct abc{
    unsigned short a:3;
    unsigned short b:2;
    unsigned short c:3;
    unsigned short d:3;
    unsigned short e:2;

} ABC;


int main(){
    ABC test;
    unsigned short x = 0x2fc3;

    memcpy(&test, &x, sizeof(x));
    printf("%x \n", test);

    test.a = 13;
    test.b = 5;
    test.c = 8;
    test.d = 6;
    test.e = 2;

    printf("%x \n", test);


    memcpy(&x, &test, 1);

    printf("%x\n", x);
    // Print("%x\n", x);
}