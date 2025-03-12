#include <stdio.h>
#include <stdint.h>

// Define a struct with bit-fields in a single byte
typedef struct {
    uint8_t flag1 : 1;  // 1-bit field // least significat bit
    uint8_t flag2 : 1;
    uint8_t mode  : 2;  // 2-bit field
    uint8_t code  : 4;  // 4-bit field  // most significant bit
} BitField8;

int main() {
    BitField8 bf;

    // Assign values
    bf.flag1 = 0;
    bf.flag2 = 0;
    bf.mode  = 2;  // 2-bit (00, 01, 10, 11)
    bf.code  = 7;  // 4-bit (0000 - 1111)

    // Print the raw byte
    printf("BitField as raw byte: 0x%02X\n", *(uint8_t*)&bf);

    return 0;
}
