#include<stdio.h>

int main(){

    int a = 6;
    int *d = &a;







    int *p;

    p = 0;

    printf("%d\n",*p);


    

    if(p == NULL){
        printf("Null Pointer\n");
    }
    else {
        printf("Not Null\n");
    }


}