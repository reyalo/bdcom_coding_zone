#include <stdio.h>
#include <stdlib.h>

int main() {
    char buffer[128];  // Buffer to store command output


    // all this run in linux
    FILE *fp = popen("date", "r");  // Open pipe to execute "date" command

    if (fp == NULL) {
        perror("popen failed");
        return 1;
    }

    // Read the output of the "date" command line-by-line
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("Current Date and Time: %s", buffer);
    }

    // Close the pipe
    pclose(fp);

    return 0;
}
