#include<stdio.h>

// A.
typedef int Array10[10];                        // Type for an array of 10 integers
typedef Array10 *PtrArray10;                    // Pointer to an array of 10 integers
typedef PtrArray10 (*FuncAA)(void *);           // Function pointer returning PtrArray10

FuncAA AA;                                      // Equivalent to: int (*(*AA)(void *))[10];


// B.
typedef void (*FuncIntVoid)(int);               // Function pointer for void func(int)
typedef FuncIntVoid (*FuncBB)(int, FuncIntVoid);// Function returning FuncIntVoid

FuncBB BB;                                      // Equivalent to: void (*BB(int, void (*)(int)))(int);



// C.
typedef char CharArray5[5];                     // Define an array of 5 chars
typedef CharArray5 *FuncCC(void);               // Function returning a pointer to array
typedef FuncCC *PtrFuncCC[3];                   // Array of 3 function pointers

PtrFuncCC CC;                                   // Equivalent to: char (*(*CC[3])())[5];
