#include <stdio.h>

#pragma pack(8)
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



struct Misaligned {          // 64 bit : size 16,  32 bit : 16
    char a;   // 1 byte // start from 
    int b;    // 4 bytes (but needs to start at a multiple of 4)
    char c;   // 1 byte
    char d;
    char e;
    short f;
};

// struct Aligned {
//     char a;   // 1 byte
//     char c;   // 1 byte
//     char e;
//     int b;    // 4 bytes (properly aligned)
// };

int main() {

    // struct Misaligned x;
    // printf("%p \n", &x);
    // printf("Size of Misaligned: %lu bytes\n", sizeof(struct Misaligned)%16);
    printf("%u %u %u %u\n", sizeof(struct Example1), sizeof(struct Example2), sizeof(struct Example3), sizeof(struct Misaligned));
    // printf("Size of Aligned: %lu bytes\n", sizeof(struct Aligned));
    return 0;
}
