//whether a right shift of a signed value is performed on
//this machine as an arithmetic or a logical shift
#include<stdio.h>

int main(){

    int num = 1;
    // int shifted_num = (num >> 1);

    char *p = (char *)&num;

    if( *p == 1){
        printf("\nLittle Endian\n");
    }
    else printf("Big Endian\n");

    return 0;
}