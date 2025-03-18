// sealed
#include <stdio.h>
#include <stdint.h>

// Define a struct with bit-fields in a single byte
typedef struct {
    uint8_t flag1 : 1;  // 1-bit field // least significat bit
    uint8_t flag2 : 1;
    uint8_t mode  : 2;  // 2-bit field
    uint8_t code  : 4;  // 4-bit field  // most significant bit
} BitField8;


typedef struct {
        uint16_t frag_offset_high:5;
        uint16_t mf_flag:1;
        uint16_t df_flag:1;
        uint16_t reserved_flag:1;
        uint16_t frag_offset_low:8;
} BitField1;



int main() {
    BitField8 bf;

    // Assign values
    bf.flag1 = 0;
    bf.flag2 = 0;
    bf.mode  = 2;  // 2-bit (00, 01, 10, 11)
    bf.code  = 7;  // 4-bit (0000 - 1111)

    // Print the raw byte
    // printf("BitField as raw byte: 0x%02X\n", *(uint8_t*)&bf);

    
    
    
    
    
    BitField1 bf1;
    char *str = (char*)&bf1;
    
    

    bf1.frag_offset_high = 7;
    bf1.mf_flag = 1;
    bf1.df_flag = 1;
    bf1.reserved_flag = 0;
    bf1.frag_offset_low = 4;

    // str[0] = 0x67;
    // str[1] = 0x04;

    // printf("frag_offset_high: %x\n", bf1);

    printf("frag_offset_high: %d\n", bf1.frag_offset_high);
    printf("mf_flag: %d\n", bf1.mf_flag);
    printf("df_flag: %d\n", bf1.df_flag);
    printf("reserved_flag: %d\n", bf1.reserved_flag);
    printf("frag_offset_low: %d\n", bf1.frag_offset_low);

    printf("byte 0 %x \n", str[0]);
    printf("byte 1 %x\n", str[1]);

    return 0;
}
