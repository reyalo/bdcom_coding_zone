#include <stdio.h>

struct Flags {
    unsigned int read  : 1;
    unsigned int write : 1;
    unsigned int exec  : 1;
    unsigned int res   : 5;  // Reserved (Padding)
};

// Function to set a flag using bitwise operations (Endianness Independent)
void set_flag(struct Flags *f, unsigned int flag) {
    *((unsigned int*)f) |= (1 << flag);
}

// Function to clear a flag
void clear_flag(struct Flags *f, unsigned int flag) {
    *((unsigned int*)f) &= ~(1 << flag);
}

// Function to check if a flag is set
int is_flag_set(struct Flags *f, unsigned int flag) {
    return (*((unsigned int*)f) & (1 << flag)) != 0;
}

int main() {
    struct Flags permissions = {0};  // Initialize all bits to 0

    set_flag(&permissions, 0);  // Set Read permission
    set_flag(&permissions, 1);  // Set Write permission

    printf("Read: %d, Write: %d, Execute: %d\n",
           is_flag_set(&permissions, 0),
           is_flag_set(&permissions, 1),
           is_flag_set(&permissions, 2));

    clear_flag(&permissions, 1);  // Clear Write permission
    printf("After clearing Write: Read: %d, Write: %d, Execute: %d\n",
           is_flag_set(&permissions, 0),
           is_flag_set(&permissions, 1),
           is_flag_set(&permissions, 2));

    return 0;
}
