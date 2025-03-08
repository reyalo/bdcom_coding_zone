#include <stdio.h>

#if defined(__GNUC__)
    #define COMPILER "GCC"
#elif defined(_MSC_VER)
    #define COMPILER "MSVC"
#else
    #define COMPILER "Unknown Compiler"
#endif

int main() {
    printf("Compiled with %s\n", COMPILER);
    return 0;
}
