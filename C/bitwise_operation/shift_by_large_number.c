#include<stdio.h>

int main(){
    int lval = 0xFEDCBA98 << 32;
    int aval = 0xFEDCBA98 >> 36;
    unsigned uval = 0xFEDCBA98u >> 40;

    printf("%x \n %x \n %x \n", lval, aval, uval);


    char ch = -35;
    printf("%x \n", ch);
    
    ch = (ch >> 1);

    printf("%x %d\n", ch, ch);

}