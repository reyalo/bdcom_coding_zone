#include<stdio.h>

typedef void (*fptr_b)(void);

void func(void){

    printf("Hello World\n");
    return;

}

int main(){

    int a = (int)func;

    fptr_b b;
    b = (fptr_b)a;

    (b)();

    return 0;
}


/*
1.
    int a = (int)func;

    ((void (*)(void))a)();



2.
    int a = (int)func;

    void (*b)(void);     

    b = (void (*)(void))a;

    (b)();

3. 

    typedef void (*fptr_b)(void);   // using typedef

    int a = (int)func;

    fptr_b b;
    b = (fptr_b)a;

    (b)();


*/