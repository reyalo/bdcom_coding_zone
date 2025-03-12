#include<stdio.h>

int main(){

    unsigned char x = 0b10011001;
    x = x >> 7;

    printf("%d \n", x);
    
    char y = 0b10011001;

    y = y >> 7;

    printf("%d \n", y);


    // logical not vs bitwise not
    int a = 5;
    int not_a = ~a;

    printf("%d = %02x\n", not_a, not_a);

    




    return 0;
}