#include <stdio.h>

// Function to store a value into specified bit positions of an integer
int store_bit_field(int original_value, int value_to_store, unsigned starting_bit, unsigned ending_bit) {
    // Step 1: Construct the mask for the bit field
    unsigned mask = ((1 << (starting_bit - ending_bit + 1)) - 1) << ending_bit;

    // Step 2: Clear the specified bits in the original value
    original_value &= ~mask;

    // Step 3: Shift the new value left so that it is aligned in the field.
    value_to_store = (value_to_store << ending_bit);

    // Step 4: AND the shifted value with the mask to ensure that
    //         it has no bits outside of the field.
    value_to_store &= mask;

    // Step 5: OR the resulting value into the original integer
    return original_value | value_to_store;
}



// Test function
int main() {
    // Test cases
    int result1 = store_bit_field(0x0, 0x1, 4, 4);
    int result2 = store_bit_field(0xFFFF, 0x123, 15, 4);
    int result3 = store_bit_field(0xFFFF, 0x123, 13, 9);

    // Print results
    printf("Result 1: 0x%X (Expected: 0x10)\n", result1);
    printf("Result 2: 0x%X (Expected: 0x123F)\n", result2);
    printf("Result 3: 0x%X (Expected: 0xC7FF)\n", result3);

    return 0;
}
