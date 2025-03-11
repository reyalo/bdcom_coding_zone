#include <stdio.h>
#include <stdlib.h> // for Proper exit Handling, EXIT SUCCESS(0), EXIT FAILURE(1)

int main() {
    FILE *input, *output;
    long file_size, start_pos;

    // Open input file for reading
    input = fopen("input.txt", "rb");
    if (input == NULL) {
        perror("Error opening input.txt");
        return EXIT_FAILURE;
    }

    // Open output file for writing
    output = fopen("output.txt", "wb");
    if (output == NULL) {
        perror("Error opening output.txt");
        fclose(input);
        return EXIT_FAILURE;
    }

    // Find the file size
    fseek(input, 0, SEEK_END);      // move pointer to end of the file
    file_size = ftell(input);       // return current position of the pointer from start

    // Calculate start position (n/2 to n)
    start_pos = file_size / 2;      // caculating middle position and set it to new start
    fseek(input, start_pos, SEEK_SET);  // set the pointer to start_pos

    // Efficiently copy using a buffer
    char buffer[4096];  // 4KB buffer
    size_t bytes_read;  // size of the input after fread() call.
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input)) > 0) {
        fwrite(buffer, 1, bytes_read, output);
    }

    // Close files
    fclose(input);
    fclose(output);

    printf("Successfully copied second half of input.txt to output.txt\n");

    return EXIT_SUCCESS;
}
