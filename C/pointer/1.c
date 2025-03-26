#include<stdio.h>


void stcpy(char *s, char *t){
    while((*s++ = *t++) != '\0');
}


int main(){

    char s[10];
    char *t = "heloo";

    printf("%c\n", *t++);
    stcpy(s, t);

    puts(s);

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