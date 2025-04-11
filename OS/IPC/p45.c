#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define NUM_PROCESSES 5

int main() {
    // Shared memory for counter
    sem_unlink(sem);
    int *shared_counter = mmap(NULL, sizeof(int),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Shared memory for semaphore
    sem_t *sem = mmap(NULL, sizeof(sem_t),
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shared_counter == MAP_FAILED || sem == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    *shared_counter = 0;
    sem_init(sem, 1, 1); // 1 means shared between processes

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            sem_wait(sem);  // Lock
            (*shared_counter)++;
            printf("Process %d (PID: %d) incremented counter to %d\n",
                   i + 1, getpid(), *shared_counter);
            sem_post(sem);  // Unlock
            exit(0); // Child exits
        }
    }

    // Parent process waits for all children
    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    printf("Final value of shared_counter: %d\n", *shared_counter);

    // Cleanup
    sem_destroy(sem);
    munmap(shared_counter, sizeof(int));
    munmap(sem, sizeof(sem_t));

    return 0;
}
