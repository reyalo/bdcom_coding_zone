#include <stdlib.h>
#include <stdio.h>

int main(int ac, char **av) {
    int exit_status = EXIT_SUCCESS;
    FILE *input;
    char line[256];  // Buffer to store each line of the file

    // Check if there are no files provided
    if (ac == 1) {
        fprintf(stderr, "No files provided\n");
        return EXIT_FAILURE;
    }

    // Process each file provided in the command-line arguments
    while (*++av != NULL) {
        // Try opening the file
        input = fopen(*av, "r");
        if (input == NULL) {
            perror(*av);  // If file cannot be opened, print an error
            exit_status = EXIT_FAILURE;
            continue;
        }

        printf("Processing file: %s\n", *av);

        // Read the file line by line and process it
        while (fgets(line, sizeof(line), input)) {
            // Process each line (e.g., print it to the console)
            printf("%s \n", line);  // Print the line to stdout
        }

        // Check for any errors while reading the file
        if (ferror(input)) {
            perror("Error reading the file");
            exit_status = EXIT_FAILURE;
        }

        // Close the file
        if (fclose(input) != 0) {
            perror("fclose");
            exit(EXIT_FAILURE);
        }
    }

    // Return the final exit status
    return exit_status;
}
