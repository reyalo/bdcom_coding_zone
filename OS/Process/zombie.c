#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process
        printf("Parent process sleeping... PID = %d\n", getpid());
        sleep(60);              // Give time to observe the zombie

    } else if (pid == 0) {
        // Child process
        printf("Child process exiting... PID = %d\n", getpid());
        exit(0);               // Exits, becomes zombie until parent collects it
    } else {
        perror("fork");
    }

    return 0;
}
