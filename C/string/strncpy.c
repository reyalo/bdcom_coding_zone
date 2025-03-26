#include <stdio.h>
#include <string.h>

void print_c(char *str) {
    for (int i = 0; i < 40; i++) {
        printf("str[%d]: %c\n", i, str[i]);
    }
}
void print_n(char *str) {
    for (int i = 0; i < 40; i++) {
        printf("str[%d]: %d\n", i, str[i]);
    }
}

int main() {
    
    char str1[] = "hello";
    char str2[] = "world_bangladesh";

    strncpy(str1, str2, sizeof(str1));      // strncpy() will copy without null terminator

    printf("str1: %s\n", str1);             // output: world
    printf("str2: %s\n", str2);             // output: world_bangladesh

    strncpy(str1, str2, sizeof(str1)-1);    // string may be null terminated or not, it depends string elements before this line.

    printf("str1: %s\n", str1);             // output: undefined, // world_world_bangladesh

    print_c(str1);




    return 0;
};

//!case : 1
    // char str1[] = "hello";                   // str1 = "hello\0"
    // char str2[] = "world_bangladesh";        // str2 = "world_bangladesh\0"

    // strncpy(str1, str2, sizeof(str1));       // strncpy() will copy "world_" without null terminator

    //? output(str1): undefined, // world_world_bangladesh

//!case : 3
    // char str1[] = "hello";
    // char str2[] = "world_bangladesh";

    // strncpy(str1, str2, sizeof(str1));      // strncpy() will copy without null terminator

    // strncpy(str1, str2, sizeof(str1)-1);    // string may be null terminated or not, it depends strings last elements before this line.

    //? output : undefined, // world_world_bangladesh


//!case : 4
    // char str1[] = "hello";
    // char str2[] = "world_bangladesh";

    // strncpy(str1, str2, sizeof(str1)-1);    // string null terminated, reason last string elements before this line is '\0'.
    //? output: world;