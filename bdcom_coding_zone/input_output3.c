/* 
** Convert a series of digits from the standard input to an integer. 
*/ 
#include <stdio.h> 
#include <ctype.h> 

int main()
{ 
    int value; 
    int ch;
    char *input = "we shal over";
    printf("%s", input);
    // value = 0; 
    //     /* 
    //     ** Convert digits from the standard input; stop when we get a 
    //     ** character that is not a digit. 
    //     */ 
    //     while( ( ch = getchar() ) != EOF && isdigit(ch)){ 
    //         value *= 10; 
    //         value += ch - '0'; 
    //     }
    //     /* 
    //     ** Push back the nondigit so we don't lose it. 
    //     */ 
    //     // ungetc( ch, stdin );
        
    return 0; 
}