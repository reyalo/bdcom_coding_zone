#include <stdio.h>

union {
    unsigned int i;
    char c[4];
} test = {0x01020304};


int main() {

    //using uninon
    if (test.c[0] == 0x04)
        printf("Little-endian\n");
    else
        printf("Big-endian\n");


    //using pointer
    unsigned int num = 0x01020304;
    char *ptr = (char *)&num;

    if (*ptr == 0x04)
        printf("Little-endian\n");
    else
        printf("Big-endian\n");


    //using bitwise operation
    unsigned int num2 = 1;
    if ((num2 >> 24) & 0xFF)  // MSB should be 1 in big-endian
        printf("Big-endian\n");
    else
        printf("Little-endian\n");



    //using file
    FILE *file = fopen("test.bin", "wb");
    unsigned int num3 = 0x01020304;
    fwrite(&num3, sizeof(num3), 1, file);   // wirte 1 elements of 4 byte
    fclose(file);

    file = fopen("test.bin", "rb");
    char c[4];
    fread(c, 1, 4, file);                   // read 4 elements of 1 byte
    fclose(file);

    if (c[0] == 0x04)
        printf("Little-endian\n");
    else
        printf("Big-endian\n");

    return 0;   
}
