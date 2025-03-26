#include <stdio.h>



int main(void){
    int x;
    if((x = 2) || (x = 3)){
        printf("%d\n", x);
    }

    return 0;
}