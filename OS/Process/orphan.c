// orphan.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process exits
        sleep(20);
        printf("Parent (PID: %d) exiting...\n", getpid());
        sleep(20);
        exit(0);
    } else if (pid == 0) {
        // Child process sleeps long enough to be seen as orphan
        sleep(20);
        printf("Child (PID: %d), Parent (PPID: %d)\n", getpid(), getppid());
    }

    return 0;
}
