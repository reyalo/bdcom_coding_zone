#include <stdio.h>



int main(void){
    int n;
    scanf("%d", &n);
    bool leap_year = ((n % 400 == 0) ||((n % 4 == 0) && (n % 100 != 0)) );

    

    int a = 20;

    if( 1 <= a <= 10 )
            printf( "In range\n" );
    else
            printf( "Out of range\n" );

    return 0;
}