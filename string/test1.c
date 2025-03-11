#include <stdio.h>
#include <stdint.h>

#define N 8  // Number of people


// Define a struct to hold a 32-bit number using four 8-bit values (MSB -> LSB)
typedef struct {
    uint8_t bytes[4];  // 4 bytes for the 32-bit value (MSB -> LSB)
} UInt32;

// Function to add two UInt32 numbers (correct handling of overflow/carry)
void add_uint32(UInt32 *sum, UInt32 num) {
    uint8_t carry = 0, temp_carry;

    // Add each byte from MSB to LSB, considering the carry
    for (int i = 3; i >= 0; i--) {

        // sum->bytes[i] = (sum->bytes[i] + num.bytes[i]) + carry;
        // carry         = carry_generate_from [(sum->bytes[i] + num.bytes[i]) + carry]);

        uint8_t temp = sum->bytes[i] + num.bytes[i];

        temp_carry = (temp < sum->bytes[i]) || (temp < num.bytes[i]);
                                      // Carry occurs if the sum overflows

        uint8_t temp2 = temp + carry;

        temp_carry += (temp2 < temp) || (temp2 < carry);
                                      // Carry occurs if the sum overflows


        
        
        // final
        carry = temp_carry;
        sum->bytes[i] = temp2;
        
    }

}

// Function to divide a UInt32 number by 8 using shifts (without exceeding byte limits)
UInt32 divide_by_8(UInt32 sum, uint8_t *remainder) {
    UInt32 result = sum;
    // uint8_t remainder = 0;

    // Perform the division from MSB to LSB
    for(int j = 0; j < 3; ++j){
        for (int i = 3; i>= 0; i--) {

            if(i < 3 && (result.bytes[i]&1)){
                result.bytes[i+1] |=(1 << 7); 
                                // set bit 1 on most significant bit of right hand bytes
            }
            if(i == 3){ 
                *remainder >>= 1;
                if(result.bytes[i]&1){
                    *remainder |= (1 << 7);     
                                //set bit 1 on most significant bit of remainder bytes
                }
            }

            result.bytes[i] >>= 1;
        }
    }

    return result;

    
}


int main() {
    // Define scores as 32-bit values using the struct (MSB -> LSB)
    UInt32 scores[N] = {
        {{0, 0, 0, 210}},   // 210
        {{0, 0, 1, 71}},    // 327
        {{0, 0, 1, 157}},   // 413
        {{0, 0, 223, 57}},  // 57145
        {{0, 0, 35, 194}},  // 9154
        {{0, 0, 0, 163}},   // 163
        {{0, 0, 90, 132}},  // 23172
        {{0, 0, 15, 241}}   // 4081
    };

    // Initialize the total sum as zero
    UInt32 total_sum = {{0, 0, 0, 0}};
    
    // Sum all the scores
    for (uint8_t i = 0; i < N; i++) {
        add_uint32(&total_sum, scores[i]);

    }

    // Divide the total sum by 8
    uint8_t remainder = 0;
    UInt32 avg = divide_by_8(total_sum, &remainder); 
                        // passing total sum and remainder address and returned avg

    // Print the average score in hex format with remainder
    printf("Average Score: 0x%x%x%x%x.%x\n",
        avg.bytes[0], avg.bytes[1], avg.bytes[2], avg.bytes[3], remainder);

    
    return 0;
}
