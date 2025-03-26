#include<stdio.h>
#include<string.h>


// #pragma pack(1)
// typedef struct AA{
//     unsigned char a:3;
//     unsigned short b:4;
//     unsigned int c:9;
// } _a;

typedef struct AA{
    unsigned char a:3;
    unsigned short b:13;
    unsigned int c:10;
} _a;            // Le 03 ff ff ff

typedef struct BB{
    unsigned char a:4;
    unsigned short b:13;
    unsigned int c:10;
} _b;           // Le 

typedef struct CC{
    unsigned char a:3;
    unsigned short b:5;
    unsigned int c:8;
} _c;           // Le 0 0 ff ff  Be ff ff 0 0
// typedef struct AA{
//     unsigned char a:4;
//     unsigned short b:4;
//     unsigned int c:8;
// } _a;

void prp(int sz, unsigned char *p){

    union {
        unsigned int i;
        char c[4];
    } test = {0x01020304};

    if(test.c[0] == 0x01){
        for(int i=0; i < sz ; ++i){
            printf("%x ", *p++);
        }
        printf("\n");
    }
    else{
        p = p+8;
        for(int i=0; i < sz ; ++i){
            printf("%x ", *--p);
        }
        printf("\n");
    }

}


int main(){

    _c v;

    memset(&v, 0, sizeof(v));

    v.a =  7;
    v.b = (1<<6) - 1 ;
    v.c = (1<<9) - 1;


    unsigned char *p = (unsigned char *)&v;

    unsigned long long x;

    printf("%d\n", sizeof(v));
    memcpy(&x, &v, sizeof(v));
    printf("%x\n", x);

    prp(8, p);

    // unsigned long long *pp = (unsigned long long *) p;
    // printf("%x\n", *pp++);
    // // printf("%x\n", pp);

    printf("%x \n", *p++);
    printf("%x \n", *p++);
    printf("%x \n", *p++);
    printf("%x \n", *p++);
    printf("%x \n", *p++);
    printf("%x \n", *p++);
    printf("%x \n", *p++);
    printf("%x \n", *p++);

    

}