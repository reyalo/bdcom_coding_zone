#include<stdio.h>

int main(){
    int x;

    // using ternary operator
    x > 10 ? : printf("This is the else part\n");


    if (x > 10);  // Empty statement no action
    else {
        printf("This is the else part\n");
    }

    //using goto
    if (x > 10)
    goto skip_else;

    printf("This is the else part\n");

    skip_else:
    // Continue execution here if condition is true

}