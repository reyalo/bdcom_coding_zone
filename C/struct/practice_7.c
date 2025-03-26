#include<stdio.h>
#include<string.h>



struct AA{
    short a;        // offset = 0, size = 2,  
    // padding 2
    int b;          // offset = 4, size = 4,
    char c;         // offset = 8, size = 1,
    // padding 3

};  // total size = 12

struct BB{
    short a;        // offset = 0, size = 2,
    // padding 2
    struct AA aa;   // offset = 4, size = 12,
    char c;         // offset = 16, size = 1,
    // padding 3

};  // total size = 20

#pragma pack (1)
struct CC{
    short a;        // offset = 0, size = 2, 
    struct AA aa;   // offset = 2, size = 12,
    char c;         // offset = 14, size = 1,

}; // total size = 15

struct AAA{
    short a;        // offset = 0, size = 2,
    int b;          // offset = 2, size = 4,
    char c;         // offset = 6, size = 1,

};  // total size = 7

struct BBB{
    struct AAA aaa; // offset = 0, size = 7,
    char c;         // offset = 7, size = 1,

};  // total size = 8

#pragma pack ()

int main(){

    // for practice 7
    printf("\n\n%d %d %d %d %d\n", sizeof (struct AA), sizeof (struct AAA),
    sizeof (struct BB), sizeof (struct BBB), sizeof (struct CC));

    return 0;
}

    // for paractice 8
    // struct AA aa;
    // struct AAA aaa;
    // struct BB bb;
    // struct BBB bbb;
    // struct CC cc;
    // char array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    // memcpy(&aa, array, sizeof(aa));
    // memcpy(&aaa, array, sizeof(aaa));
    // memcpy(&bb, array, sizeof(bb));
    // memcpy(&bbb, array, sizeof(bbb));
    // memcpy(&cc, array, sizeof(cc));
    // printf("\n\n%d %d %d %d %d\n", aa.c, aaa.c, bb.c, bbb.c, cc.c);
    // //                             9,    7,    17,    8,   ,  15

//     return 0;
// }


