//whether a right shift of a signed value is performed on
//this machine as an arithmetic or a logical shift
#include<stdio.h>

int main(){

    int num = -1;
    int shifted_num = (num >> 1);

    if( num == shifted_num){
        printf("\nArithmatic_shift\n");
    }
    else printf("Logical Shift\n");

    return 0;
}