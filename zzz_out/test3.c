#include<stdio.h>

int main(){
    int i = 0;
    unsigned int len = 0;


    printf("%u\n", len - 4);

    if( i <= (len - 1)){
        printf("i <= len true\n");
    }
    else {
        printf("i <= len false\n");
    }

    printf("%d\n", (char)100);  // 'char' is promoted to 'int'


    char a = 50, b = 100;
    char c = a + b;  // 'a' and 'b' are promoted to 'int', sum is also 'int'
    printf("%d\n", c); // implicit convertion er somoy sign extension hoy

    unsigned char ch = 0;

    if(-1 < ch){
        printf(" Yes \n");
    }
    else printf(" No \n");

    unsigned char val = 1;
    int shifted = val << 20;  // 'val' is promoted to 'int' before shifting

    printf("%d\n", shifted);

    int j = -5;

    unsigned int ll = -1;

    // printf("%d\n", ll);

    // return 0;

    while(j <= ll){

        printf(" %d ", j);
        j++;
    }




    return 0;


}