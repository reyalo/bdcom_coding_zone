#include <stdio.h>
#include <stdlib.h>

int main() {
    char buffer[128];

    // Execute 'date' and redirect output to a temporary file
    system("date > temp.txt");  // it is only for linux os.

    // Open the temporary file for reading
    FILE *fp = fopen("temp.txt", "r");
    if (fp == NULL) {
        perror("File open failed");
        return 1;
    }

    // Read the output from the file
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("Current Date and Time: %s", buffer);
    }

    // Close the file
    fclose(fp);

    // Remove the temporary file
    remove("temp.txt");

    return 0;
}
