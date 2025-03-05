#include <stdio.h>

// Function to sum elements of an array
unsigned int sum_elements(unsigned int a[], unsigned int length) {
    unsigned int i, result = 0;

    for (i = 0; i + 429496720 <= length - 1; i++){  // Original condition
        result += a[i];
        // if(i == 0)break;
    }

    return result;
}

// Test function
void test_sum_elements() {
    unsigned int test1[] = {1, 2, 3, 4, 5};  // Normal case
    unsigned int test2[] = {};               // Edge case: empty array
    unsigned int test3[] = {10};             // Single element case
    unsigned int test4[] = {100, 200, 300};  // Small random case
    unsigned int test5[1000];                // Large case

    // Fill large test case with numbers from 1 to 1000
    for (unsigned int i = 0; i < 1000; i++)
        test5[i] = i + 1;

    // Running tests
    printf("Test 1 (1,2,3,4,5): Expected: 15, Got: %u\n", sum_elements(test1, 0));
    // printf("Test 2 (Empty array): Expected: 0, Got: %u\n", sum_elements(test2, 0));
    printf("Test 3 (Single element 10): Expected: 10, Got: %u\n", sum_elements(test3, 1));
    printf("Test 4 (100, 200, 300): Expected: 600, Got: %u\n", sum_elements(test4, 3));
    printf("Test 5 (1 to 1000): Expected: 500500, Got: %u\n", sum_elements(test5, 1000));
}

int main() {
    test_sum_elements();
    return 0;
}
