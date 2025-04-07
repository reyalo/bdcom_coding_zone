#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t zombie_pid, orphan_pid;

    // Create Zombie Process
    zombie_pid = fork();
    if (zombie_pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (zombie_pid == 0) {
        // Child process for zombie
        printf("Zombie Child: PID=%d, PPID=%d\n", getpid(), getppid());
        printf("Zombie Child: Exiting without parent calling wait()\n");
        exit(0); // Exit immediately to become zombie
    } else {
        // Parent process does NOT call wait() here
        printf("Parent: Created zombie child with PID=%d\n", zombie_pid);
        sleep(5); // Let time to observe the zombie process using `ps`
    }

    // Create Orphan Process
    orphan_pid = fork();
    if (orphan_pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (orphan_pid == 0) {
        // Child process for orphan
        sleep(6); // Wait for parent to exit
        printf("Orphan Child: PID=%d, New PPID=%d (should be 1)\n", getpid(), getppid());
        sleep(10); // Stay alive for observation
        exit(0);
    } else {
        printf("Parent: Created orphan child with PID=%d\n", orphan_pid);
        printf("Parent: Now exiting to orphan the child\n");
        sleep(2); // Ensure we don’t call wait() for the zombie
        // NOTE: zombie child still exists here
        // Don't wait for orphan child either
        exit(0); // Parent exits — orphaning the second child
    }

    return 0;
}
