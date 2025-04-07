#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    // Open a non-existent device file to induce a blocking operation
    int fd = open("/dev/non_existent_device", O_RDONLY); // This file doesn't exist
    if (fd == -1) {
        perror("Error opening device");
    }

    printf("Process will now block indefinitely. Check 'ps' for the process state.\n");

    // Simulate uninterruptible sleep by trying to read from the file
    char buffer[100];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer)); // Will block here
    if (bytesRead == -1) {
        perror("Error reading from device");
    }

    close(fd);  // Close the file (won't reach here unless the read succeeds)
    return 0;
}
