#include <stdio.h>
#include <string.h>

int main() {
    // Example for strcpy
    char source_str[] = "Hello, World!";  // A string (null-terminated)
    char dest_str[20];                     // Destination array with enough space

    strcpy(dest_str, source_str);  // Copy string (including null terminator)
    printf("\n\nstrcpy result: %s\n", dest_str);  // Output: Hello, World!

    // Example for memcpy
    char source_data[] = { 'H', 'e', 'l', 'l', 'o', 0x01, 0x02 }; // raw data
    char dest_data[10];  // Destination array

    memcpy(dest_data, source_data, sizeof(source_data)); // Copy raw data (no null terminator)
    
    printf("memcpy result: ");
    for (int i = 0; i < sizeof(source_data); i++) {
        printf("0x%02x ", dest_data[i]);  // Output: 0x48 0x65 0x6c 0x6c 0x6f 0x01 0x02
    }
    printf("\n");

    return 0;
}
