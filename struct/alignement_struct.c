#include <stdio.h>

#pragma pack(8)
struct Example {   // analysis with 8 byte alignment // expected 12
    char a;   // 1 byte
    int b;    // 4 bytes
    short c;  // 2 bytes
};

struct Example2 {
    char a;   // 1 byte
    double b; // 8 bytes
    int c;    // 4 bytes
};
struct Example3 {
    int a;   // 1 byte
    double b; // 8 bytes
    char c;    // 4 bytes
};



struct Misaligned {
    char a;   // 1 byte // start from 
    // int b;    // 4 bytes (but needs to start at a multiple of 4)
    char c;   // 1 byte
    char b;
    short d;
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
    printf("Size of Misaligned: %lu bytes\n", sizeof(struct Example3));
    // printf("Size of Aligned: %lu bytes\n", sizeof(struct Aligned));
    return 0;
}
