#include <stdio.h>

// Define DEBUG_MODE to enable debugging
//#define DEBUG_MODE

// #ifdef DEBUG_MODE
//     #define DEBUG_PRINT(fmt, expr)  printf(fmt ": %s = %d\n", #expr, expr)
// #else
//     #define DEBUG_PRINT(fmt, expr)  // Empty macro, does nothing
// #endif

// int main() {
//     int x = 10;
//     int y = 20;

//     // Use the debugging macro to print expression and its value
//     DEBUG_PRINT("Debugging", x + y);  // Outputs: Debugging: x + y = 30
//     DEBUG_PRINT("Value of x", x);      // Outputs: Value of x: x = 10
//     DEBUG_PRINT("Value of y", y);      // Outputs: Value of y: y = 20
//     printf("something_strange""another string\n");

//     return 0;
// }

//* for question 14.9.1
// void file_name(){
//     printf("\n%s\n", __FILE__);
// }
// void line_number(){
//     printf("%d\n", __LINE__);
// }
// void current_date(){
//     printf("%s\n", __DATE__);
// }
// void current_time(){
//     printf("%s\n", __TIME__);
// }

// #define DEBUG

#ifdef DEBUG
    #define debug(fmt, ...) printf("[DEBUG] File: %s, Function: %s, Line: %d -> " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#else
    #define debug(fmt, ...)
#endif


// #define DEBUG_MODE

// #ifdef DEBUG_MODE
//     #define DEBP(str, expr) printf(str " %s = %d\n", #expr, expr)
// #else
//     DEBP(expr);
// #endif

void testFunction(int x) {
    debug("Value of x: %d and %d", x, x*x);
}

int main(){

    //solve 1
    // file_name();
    // line_number();
    // current_date();
    // current_time();

    // #ifdef __STDC__
    //     printf("Ansi c compiler\n");
    // #else
    //     printf("Not Ansi C compiler\n");
    // #endif



    int a = 42;
    debug("Starting main function");
    testFunction(a);
    return 0;









    // int x = 10, y = 20;
    // DEBP("ck1", x+y);
    // DEBP("ck2", x);
    // DEBP("ck3", y);

    
}