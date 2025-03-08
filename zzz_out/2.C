#include <stdio.h>

int main() {
    int decimal = 100;        // Decimal (Base 10)
    int octal = 0144;         // Octal (Base 8) -> 0144 in octal is 100 in decimal
    int hexadecimal = 0x64;   // Hexadecimal (Base 16) -> 0x64 is 100 in decimal
    int binary = 0b1100100;   // Binary (Base 2) -> 0b1100100 is 100 in decimal (GCC/Clang extension)

    printf("Decimal: %d\n", decimal);
    printf("Octal: %o (Decimal Equivalent: %d)\n", octal, octal);
    printf("Hexadecimal: %x (Decimal Equivalent: %d)\n", hexadecimal, hexadecimal);
    
    #ifdef __GNUC__  // Check if using GCC/Clang (supports binary literals)
        printf("Binary: %d In Decimal Equivalent\n", binary);
    #else
        printf("Binary literals are not supported in standard C.\n");
    #endif

    return 0;
}
