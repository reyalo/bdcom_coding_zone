#include <stdio.h>
#include<string.h>

struct Status {
    unsigned char isOn : 1;  // 1-bit flag
    unsigned char isReady : 1;
    unsigned char hasError : 1;
    unsigned char reserved : 5; // Padding bits (optional)
};

struct A {
    unsigned short a : 2;  // 1-bit flag
    unsigned short b : 3;
    unsigned short c : 2;
    unsigned short d : 4; // Padding bits (optional)
};

struct AA {
    unsigned short a : 3;  // 1-bit flag
    unsigned short b : 2;
    unsigned short c : 3;
    unsigned short d : 3; // Padding bits (optional)
    unsigned short e : 2; // Padding bits (optional)
};


int main() {

    struct AA v;
    unsigned short *tt = (unsigned short *)&v;

    unsigned short aa;

    memset(tt, 0, 2);

    

    
    printf("%x\n", aa);
    

    unsigned short ta = 0x2fc3;

    memcpy(&v, &ta, sizeof(ta));

    v.a = 13;
    v.b = 5;
    v.c = 8;
    v.d = 6;
    v.e = 2;
    
    memcpy(&aa, &v, sizeof(aa));

    printf("%x\n", aa);



    // printf("%x\n", *tt);



    // struct Status s = {1, 0, 1}; // isOn=1, isReady=0, hasError=1
    // printf("\n\nSize of struct: %lu bytes\n", sizeof(s)); // Expected: 1 byte
    // printf("isOn: %d, isReady: %d, hasError: %d\n", s.isOn, s.isReady, s.hasError);
    return 0;
}
