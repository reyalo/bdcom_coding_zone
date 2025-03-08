#include <stdio.h>

//macro defination
#ifdef DEBUG
    #define debug(fmt, ...) printf("[DEBUG] File: %s, Function: %s, Line: %d -> " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#else
    #define debug(fmt, ...)
#endif

void test_function(int x) {
    debug("Value of x: %d square = %d", x, x*x);
}

int main(){

    int a = 42;
    debug("Starting main function");
    test_function(a);

    return 0;
}