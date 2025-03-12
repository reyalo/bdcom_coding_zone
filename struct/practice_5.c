#include <stdio.h>

struct Status {
    unsigned char isOn : 1;  // 1-bit flag
    unsigned char isReady : 1;
    unsigned char hasError : 1;
    unsigned char reserved : 5; // Padding bits (optional)
};

int main() {
    struct Status s = {1, 0, 1}; // isOn=1, isReady=0, hasError=1
    printf("\n\nSize of struct: %lu bytes\n", sizeof(s)); // Expected: 1 byte
    printf("isOn: %d, isReady: %d, hasError: %d\n", s.isOn, s.isReady, s.hasError);
    return 0;
}
