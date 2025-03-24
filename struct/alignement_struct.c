#include <stdio.h>

//#pragma pack(8)
struct Example1 {   // 64 bit: expected 12 // 32 bit: 12
    char a;   // 1 byte
    int b;    // 4 bytes
    short c;  // 2 bytes
};

struct Example2 {           // 64 bit : size: 24, 32 bit : 16
    char a;   // 1 byte
    double b; // 8 bytes
    int c;    // 4 bytes
};
struct Example3 {           // 64 bit : size  24, 32 bit : 16
    int a;   // 1 byte
    double b; // 8 bytes
    char c;    // 4 bytes
};

struct Example4 {           // 64 bit : size  24, 32 bit : 16
    int a;          // 4 byte
    // 4 
    long long b; // 8 bytes
    char c;    // 1 bytes
    // 1 
    short d;   // 2
};


struct Misaligned {          // 64 bit : size 16,  32 bit : 16
    char a;   // 1 byte // start from 
    int b;    // 4 bytes (but needs to start at a multiple of 4)
    char c;   // 1 byte
    char d;
    char e;
    short f;
};

enum RGB
{
    R = 100000,
    G,
    B
};

// struct Aligned {
//     char a;   // 1 byte
//     char c;   // 1 byte
//     char e;
//     int b;    // 4 bytes (properly aligned)
// };


struct AA{           // 64 bit : size  24, 32 bit : 16
    int a;          // 4 byte
    long long b; // 8 bytes
    char c;    // 1 bytes
    // 1 
    short d;   // 2
};

struct BB{           // 64 bit :40 , 32 bit : 24
    short a;          // 4 byte
    // 2
    struct AA aa;       // 32: 16(4) 64:24(8)

    char c;    // 1 bytes
    // 3
};

#pragma pack(1)

struct CC{          // 32:19 64: 27
    short a;
    struct AA aa;   // 32:16(4), 64:24(8)
    char c;
};

struct AAA{         // 32 : 19 64:27
    short a;
    struct AA aa;   // 32:16(4)
    char c;
};

struct BBB{         // 32:20  64:28
    struct AAA aaa; // 32:19
    char c;         
};

#pragma pack();

int main() {

    printf("%lu\n", sizeof(enum RGB));

    // printf("Size of Misaligned: %lu bytes\n", sizeof(struct Misaligned)%16);
    printf("%lu %lu %lu %lu %lu\n", sizeof(struct AA), sizeof(struct BB), sizeof(struct CC), sizeof(struct AAA), sizeof(struct BBB));
    // printf("Size of Aligned: %lu bytes\n", sizeof(struct Aligned));
    return 0;
}
