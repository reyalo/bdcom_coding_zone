#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    pid_t pid = vfork();  // Parent suspends until child _exit or exec

    if (pid < 0) {
        perror("vfork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("Child process PID: %d\n", getpid());
        pause(); // Wait for parent to attach (or detach)

    }
    else {
        // Parent process
        printf("Parent process PID: %d\n", getpid());

        exit(1);
    }

    // Parent (won't run until child exits due to vfork)
    return 0;
}
