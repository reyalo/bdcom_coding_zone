#include <stdio.h>

#define BYTE_SIZE 8  // Number of bits in a byte

// Set a specific bit to 1
void set_bit(char bit_array[], unsigned bit_number) {
    unsigned byte_number = bit_number / BYTE_SIZE;
    unsigned bit_offset = bit_number % BYTE_SIZE;

    bit_array[byte_number] |= (1 << (bit_offset));
}

// Clear a specific bit (set to 0)
void clear_bit(char bit_array[], unsigned bit_number) {
    unsigned byte_number = bit_number / BYTE_SIZE;
    unsigned bit_offset = bit_number % BYTE_SIZE;

    bit_array[byte_number] &= ~(1 << (bit_offset));
}

// Assign a specific bit: 0 for clear, 1 for set
void assign_bit(char bit_array[], unsigned bit_number, int value) {
    if (value)
        set_bit(bit_array, bit_number);
    else
        clear_bit(bit_array, bit_number);
}

// Test a specific bit (returns 1 if set, 0 otherwise)
int test_bit(char bit_array[], unsigned bit_number) {
    unsigned byte_number = bit_number / BYTE_SIZE;
    unsigned bit_offset = bit_number % BYTE_SIZE;

    return (bit_array[byte_number] & (1 << bit_offset)) ? 1 : 0;
}

// Function to print bits of the array (for debugging)
void print_bits(char bit_array[], int size) {
    for (int i = 0; i < size * BYTE_SIZE; i++) {
        printf("%d", test_bit(bit_array, i));
        if (i % BYTE_SIZE == BYTE_SIZE - 1) printf(" ");  // Space every byte
    }
    printf("\n");
}

// Test the functions
int main() {
    char bit_array[2] = {0};  // 16-bit array (2 bytes)
    
    set_bit(bit_array, 3);     // Set bit 3
     // Print bit array
     printf("Bit array after set_bit 3: ");
     print_bits(bit_array, 2);

    clear_bit(bit_array, 3);   // Clear bit 3

     // Print bit array
     printf("Bit array after clear bit 3: ");
     print_bits(bit_array, 2);

    assign_bit(bit_array, 7, 1);  // Assign bit 7 to 1

     // Print bit array
     printf("Bit array after assign_bit 7 for value 1: ");
     print_bits(bit_array, 2);

    // Check bit states
    printf("Bit 7: %d\n", test_bit(bit_array, 7));  // Should be 1
    printf("Bit 3: %d\n", test_bit(bit_array, 3));  // Should be 0

    return 0;
}
