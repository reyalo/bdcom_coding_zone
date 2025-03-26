#include <stdio.h>

int main() {
    // Step 1: Declare arrays for demonstration
    char char_arr[]     = {'A', 'B'};
    int int_arr[]       = {1, 2};
    long long_arr[]     = {1L, 2L};
    float float_arr[]   = {1.0f, 2.0f};
    long long ll_arr[]  = {1LL, 2LL};
    double double_arr[] = {1.0, 2.0};

    // Step 2: Initialize pointers to arrays
    char *char_ptr      = char_arr;
    int *int_ptr        = int_arr;
    float *float_ptr    = float_arr;
    double *double_ptr  = double_arr;
    long *long_ptr      = long_arr;
    long long *ll_ptr   = ll_arr;

    
    // Step 3: Display step sizes using pointer arithmetic
    printf("Type        Step Size \n\n");
     
    // Calculate actual byte differences using pointer addresses
    printf("char      |    %u\n", (char*)(char_ptr + 1) - (char*)char_ptr);
    printf("int       |    %u\n", (char*)(int_ptr + 1) - (char*)int_ptr);
    printf("float     |    %u\n", (char*)(float_ptr + 1) - (char*)float_ptr);
    printf("double    |    %u\n", (char*)(double_ptr + 1) - (char*)double_ptr);
    printf("long      |    %u\n", (char*)(long_ptr + 1) - (char*)long_ptr);
    printf("long long |    %u\n", (char*)(ll_ptr + 1) - (char*)ll_ptr);

    return 0;
}