#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>


// Data Organization

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef enum {false = 0, true} bool;


struct bitmap
{
    struct bitmap *bm_self;
    u16 max_value;      // the value used when creating a bitmap
    u16 first_value;    // the first bit has been set
    u16 last_value;     // the last bit has been set
    u16 numbers;        // numbers of '1' in buf[]
    u16 buf_len;        
    u32 buf[0];
};

static bool bitmap_check(struct bitmap *bm);
// intput : capacity, the capacity of the bitmap that will be created
// return : success bitmap / faild null
struct bitmap* bitmap_create(u16 capacity) {
    if (capacity == 0) return NULL;
    
    u16 buf_len = (capacity + 31) / 32;  // Calculate buffer length in 32-bit words
    struct bitmap *bm = (struct bitmap *)malloc(sizeof(struct bitmap) + buf_len * sizeof(u32));
    
    if (bm) {
        bm->bm_self = bm;
        bm->max_value = capacity - 1;
        bm->first_value = 0;
        bm->last_value = 0;
        bm->numbers = 0;
        bm->buf_len = buf_len;
        memset(bm->buf, 0, buf_len * sizeof(u32));
    }
    return bm;
}

// input : bm, A bitmap that will be destroyed
// return : Success none / failed remind
void bitmap_destroy(struct bitmap *bm) {
    if (bitmap_check(bm)) {
        free(bm);
    }
}

// input : bm, A bitmap that need to check
// return : success true / failed false
static bool bitmap_check(struct bitmap *bm) {
    return (bm && bm->bm_self == bm);
}

// input : bm, The bitmap to which values are added
//         value, A value that will be added into the bitmap
//return : Success true / Failed false
bool bitmap_add_value(struct bitmap *bm, u16 value) {
    if (!bitmap_check(bm) || value > bm->max_value) {
        return false;
    }

    u16 word_index = value / 32;
    u16 bit_index = value % 32;
    u32 bit_mask = 1U << bit_index;

    if (!(bm->buf[word_index] & bit_mask)) {
        bm->buf[word_index] |= bit_mask;
        bm->numbers++;
        
        if (bm->numbers == 1) {
            bm->first_value = value;
            bm->last_value = value;
        } else {
            if (value < bm->first_value) bm->first_value = value;
            if (value > bm->last_value) bm->last_value = value;
        }
    }
    return true;
}

// input : bm, The bitmap to which values are removed
//         value, A value that will be removed from the specified bitmap
//return : Success true / Failed false
bool bitmap_del_value(struct bitmap *bm, u16 value) {
    if (!bitmap_check(bm) || value > bm->max_value) {
        return false;
    }

    u16 word_index = value / 32;
    u16 bit_index = value % 32;
    u32 bit_mask = 1U << bit_index;

    if (bm->buf[word_index] & bit_mask) {
        bm->buf[word_index] &= ~bit_mask;
        bm->numbers--;
        
        // Update first_value and last_value if needed
        if (bm->numbers > 0) {
            if (value == bm->first_value) {
                // Find new first value
                for (u16 i = value + 1; i <= bm->max_value; i++) {
                    if (bitmap_add_value(bm, i)) {
                        bm->first_value = i;
                        break;
                    }
                }
            }
            if (value == bm->last_value) {
                // Find new last value
                for (u16 i = value - 1; i >= 0; i--) {
                    if (bitmap_add_value(bm, i)) {
                        bm->last_value = i;
                        break;
                    }
                }
            }
        }
    }
    return true;
}

// input : bm, A bitmap that will be printed
// Return : Success none / Failed none
void bitmap_print(struct bitmap *bm) {
    if (!bitmap_check(bm)) {
        printf("Invalid bitmap\n");
        return;
    }

    printf("Bitmap Info:\n");
    printf("Max Value: %u\n", bm->max_value);
    printf("First Value: %u\n", bm->first_value);
    printf("Last Value: %u\n", bm->last_value);
    printf("Number of set bits: %u\n", bm->numbers);
    printf("Buffer length: %u\n", bm->buf_len);

    printf("Set bits: ");
    for (u16 i = 0; i <= bm->max_value; i++) {
        u16 word_index = i / 32;
        u16 bit_index = i % 32;
        if (bm->buf[word_index] & (1U << bit_index)) {
            printf("%u ", i);
        }
    }
    printf("\n");
}

// input : str, A string that will be parsed to a bitmap
// return : Success, A new bitmap stores the data is parsed from the sepcified string/ failed none.
struct bitmap *bitmap_parse_str(u8 *str) {
    if (!str) return NULL;

    u16 temp_storage[100];
    u16 cnt = 0;

    // Find maximum value in string
    u16 max_value = 0;
    char *token = strtok((char*)str, ",");
    while (token) {

        // printf("\n\ndebug: %s\n\n", token);

        u16 value = atoi(token);
        temp_storage[cnt++] = value;
        if (value > max_value) max_value = value;
        token = strtok(NULL, ",");
    }

    // Create bitmap
    struct bitmap *bm = bitmap_create(max_value + 1);
    if (!bm) return NULL;

    for(int i = 0; i < cnt; ++i){
        bitmap_add_value(bm, temp_storage[i]);
    }
    

    return bm;
}

// Helper function to print bitmap in discrete format
void print_discrete_format(struct bitmap *bm) {
    if (!bitmap_check(bm)) return;
    
    bool first = true;
    for (u16 i = 0; i <= bm->max_value; i++) {
        u16 word_index = i / 32;
        u16 bit_index = i % 32;
        if (bm->buf[word_index] & (1U << bit_index)) {
            printf("%s%u", first ? "" : ", ", i);
            first = false;
        }
    }
    printf("\n");
}

// Helper function to print bitmap in range format
void print_range_format(struct bitmap *bm) {
    if (!bitmap_check(bm)) return;
    
    bool in_range = false;
    u16 range_start = 0;
    
    for (u16 i = 0; i <= bm->max_value + 1; i++) {
        u16 word_index = i / 32;
        u16 bit_index = i % 32;
        bool is_set = (i <= bm->max_value) && 
                     (bm->buf[word_index] & (1U << bit_index));
        
        if (is_set && !in_range) {
            range_start = i;
            in_range = true;
        } else if (!is_set && in_range) {
            if (range_start == i - 1)
                printf("%u, ", range_start);
            else
                printf("%u-%u, ", range_start, i - 1);
            in_range = false;
        }
    }
    printf("\b\b \n");  // Remove last comma
}

int main() {
    // Test 1: Create and Add Values
    printf("\nTest 1: Creating bitmap and adding values\n");
    printf("----------------------------------------\n");
    struct bitmap* bm1 = bitmap_create(32);  // Create bitmap with capacity 32
    
    bitmap_add_value(bm1, 5);
    bitmap_add_value(bm1, 10);
    bitmap_add_value(bm1, 15);
    bitmap_print(bm1);  // Should show bits set at 5, 10, 15

    // Test 2: Remove Values
    printf("\nTest 2: Removing values\n");
    printf("----------------------------------------\n");
    bitmap_del_value(bm1, 10);
    bitmap_print(bm1);  // Should show bits set at 5, 15

    // Test 3: Parse String
    printf("\nTest 3: Creating bitmap from string\n");
    printf("----------------------------------------\n");
    u8 str[] = "1,3,5,7,9";
    struct bitmap* bm2 = bitmap_parse_str(str);
    bitmap_print(bm2);  // Should show bits set at 1,3,5,7,9

    // Test 4: Edge Cases
    printf("\nTest 4: Testing edge cases\n");
    printf("----------------------------------------\n");
    bitmap_add_value(bm1, 33);  // Should fail (beyond capacity)
    bitmap_add_value(NULL, 5);   // Should fail (null bitmap)
    bitmap_del_value(bm1, 20);   // Should succeed but no change (bit not set)

    // Cleanup
    bitmap_destroy(bm1);
    bitmap_destroy(bm2);

    // Create test bitmap
    struct bitmap *bm = bitmap_create(100);
    
    // Add some test values
    u16 test_values[] = {31, 33, 35, 99};
    for (int i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        bitmap_add_value(bm, test_values[i]);
    }
    
    printf("\nTest Case 1 - Discrete Format:\n");
    printf("----------------------------\n");
    print_discrete_format(bm);
    bitmap_destroy(bm);
    
    // Create another test bitmap for ranges
    bm = bitmap_create(100);
    // Add values 1-34
    for (u16 i = 1; i <= 34; i++) {
        bitmap_add_value(bm, i);
    }
    // Add values 36-49
    for (u16 i = 36; i <= 49; i++) {
        bitmap_add_value(bm, i);
    }
    // Add values 51-100
    for (u16 i = 51; i <= 100; i++) {
        bitmap_add_value(bm, i);
    }
    
    printf("\nTest Case 2 - Range Format:\n");
    printf("-------------------------\n");
    print_range_format(bm);
    
    bitmap_destroy(bm);
    return 0;
}