#include <stdio.h>

#define ARR_NUM 3

// Typedefs for different pointer types
typedef int* IntPtr;
typedef int** IntPtrPtr;
typedef int (*ArrayPtr)[5];
typedef int* PtrArray[5];

// For array of pointers (a)
void funca1(int a[]) {}          // Equivalent to int *a
void funca2(int a[ARR_NUM]) {}   // Also equivalent to int *a
void funca3(int *a) {}           // Pointer notation (same as int a[])
void funca4(int a[3]) {}         // Fixed-size (still treated as int *)
void funca4(void *a) {}         // Fixed-size (still treated as int *)
void funca5(int (*a)) {}         // Explicit pointer notation

// For array of pointers (b)
void funcb1(int *b[5]);          // Array of pointers with size
void funcb2(int *b[]);           // Array of pointers
void funcb3(int **b);            // Pointer to pointer
void funcb4(int *(*b));          // Explicit pointer notation
void funcb5(void *b);            // Void pointer
void funcb6(PtrArray b);         // Typedef array of pointers
void funcb7(IntPtrPtr b);        // Typedef pointer to pointer

// For pointer to array (c)
void funcc1(int (*c)[5]);        // Pointer to array
void funcc2(int c[][5]);         // Array form
void funcc3(int c[3][5]);        // With first dimension
void funcc4(void *c);            // Void pointer
void funcc5(ArrayPtr c);         // Typedef pointer to array

// For 2D array (d)
void funcd1(int d[4][5]);        // Full dimensions
void funcd2(int d[][5]);         // Partial dimensions
void funcd3(int (*d)[5]);        // Pointer to array
void funcd4(void *d);            // Void pointer
void funcd5(ArrayPtr d);         // Typedef pointer to array

// For pointer to pointer (e)
void funce1(int **e);            // Standard form
void funce2(int *(*e));          // Explicit pointer form
void funce3(void *e);            // Void pointer
void funce4(IntPtrPtr e);        // Typedef pointer to pointer

int main(int argc, char *argv[], char *envp[])
{
    int a[ARR_NUM] = {1, 2, 3};
    int *b[5];
    int (*c)[5];
    int d[4][5];
    int **e;

    // Passing a one-dimensional array `a` to different functions
    funca1(a);
    funca2(a);
    funca3(a);
    funca4(a);
    funca4(a);
    funca6(a);

    // Array of pointers (b) calls
    funcb1(b);
    funcb2(b);
    funcb3(b);
    funcb4(b);
    funcb5(b);
    funcb6(b);
    funcb7(b);

    // Pointer to array (c) calls
    funcc1(c);
    funcc2(c);
    funcc3(c);
    funcc4(c);
    funcc5(c);

    // 2D array (d) calls
    funcd1(d);
    funcd2(d);
    funcd3(d);
    funcd4(d);
    funcd5(d);

    // Pointer to pointer (e) calls
    funce1(e);
    funce2(e);
    funce3(e);
    funce4(e);

    return 0;
}
