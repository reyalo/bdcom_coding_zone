#include <stdio.h>
#include <stdint.h>

#define N 8  // Number of people

// Define a struct to hold a 32-bit number using four 8-bit values (MSB to LSB)
typedef struct {
    uint8_t bytes[4];  // 4 bytes for the 32-bit value
} UInt32;

// Function to add two UInt32 numbers
void add_uint32(UInt32 *sum, UInt32 num) {
    uint8_t carry = 0;
    
    // Add each byte from MSB to LSB, considering the carry
    for (int i = 3; i >= 0; i--) {
        uint8_t temp = sum->bytes[i] + num.bytes[i] + carry;
        carry = (sum->bytes[i] > 255 - num.bytes[i]) || (temp < sum->bytes[i]);
        sum->bytes[i] = temp;
    }
}

// Function to divide a UInt32 number by 8 using shifts
UInt32 divide_by_8(UInt32 sum) {
    UInt32 result = {{0, 0, 0, 0}};
    uint8_t remainder = 0;

    // Perform the division from MSB to LSB
    for (int i = 0; i < 4; i++) {
        uint8_t temp = (remainder << 8) | sum.bytes[i];  // Combine previous remainder
        result.bytes[i] = temp >> 3;  // Divide by 8 (right shift by 3)
        remainder = temp & 0x07;  // Store the remainder (last 3 bits)
    }

    return result;
}

// Function to print a UInt32 number as a decimal value by combining the bytes
void print_uint32(UInt32 num) {
    // We need to combine bytes to get the value without using uint32_t
    uint8_t byte1 = num.bytes[0];
    uint8_t byte2 = num.bytes[1];
    uint8_t byte3 = num.bytes[2];
    uint8_t byte4 = num.bytes[3];

    // Now, manually combine bytes into a 32-bit integer (without uint32_t)
    // Perform each byte addition step by shifting and adding
    uint8_t result = byte4;
    result = (result + (byte3 << 8)) & 0xFF;
    result = (result + (byte2 << 16)) & 0xFF;
    result = (result + (byte1 << 24)) & 0xFF;
    printf("%u\n", result);
}

int main() {
    // Define scores as 32-bit values using the struct (MSB -> LSB)
    UInt32 scores[N] = {
        {{0, 0, 0, 210}},   // 210
        {{0, 0, 1, 71}},    // 327
        {{0, 0, 1, 157}},   // 413
        {{0, 0, 223, 25}},  // 57145
        {{0, 0, 35, 218}},  // 9154
        {{0, 0, 0, 163}},   // 163
        {{0, 0, 90, 172}},  // 23172
        {{0, 0, 15, 241}}   // 4081
    };

    // Initialize the total sum as zero
    UInt32 total_sum = {{0, 0, 0, 0}};
    
    // Sum all the scores
    for (int i = 0; i < N; i++) {
        add_uint32(&total_sum, scores[i]);
    }

    // Divide the total sum by 8
    UInt32 avg = divide_by_8(total_sum);

    // Print the average score
    printf("Average Score: ");
    print_uint32(avg);

    printf("%x%x%x%x \n", avg.bytes[0], avg.bytes[1], avg.bytes[2], avg.bytes[3]);
    printf("%x%x%x%x \n", total_sum.bytes[0], total_sum.bytes[1], total_sum.bytes[2], total_sum.bytes[3]);
    
    return 0;
}
