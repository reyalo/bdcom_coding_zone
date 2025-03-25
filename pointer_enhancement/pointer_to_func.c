#include<stdio.h>

typedef int *a;

// Equivalent to: int (*(*AA)(void *))[10];

typedef int (*return_type)[10];                         // typedef for return type
typedef return_type (*type_AA)(void *);                 // write return type then inner most part(with new name)


// Equivalent to: void (*BB(int, void (*)(int)))(int);   // BB is a funciton which return type is a function pointer.

typedef void (*type_void_fptr)(int);                     // typedef for return type.
typedef type_void_fptr (*type_BB(int, void (*)(int)));   // write return type then, inner part(with new name)


// Equivalent to: char (*(*CC[3])())[5];
typedef char (*ret_type_cc)[5];                         // typedef for return type
typedef ret_type_cc (*type_CC)(void);                   // adding new name and most inner part




int main(){
    int b = 10;

    a ptr;
    ptr = &b;

    printf("%d\n", *ptr);
}


