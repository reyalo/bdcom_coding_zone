#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#define NUM_PROCESSES 5
#define SEM_NAME "/order_sync_semaphore"


int main() {
    
    // Create shared memory for `turn`
    int *turn = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    int *shared_counter = mmap(NULL, sizeof(int),
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    *shared_counter = 0;
    *turn = 1;  // Start with Process 1

    sem_unlink(SEM_NAME);
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // printf("Process %d (PID %d): Entering critical section.\n", 0, getpid());

    for (int i = 1; i <= NUM_PROCESSES; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            while (1) {
                sem_wait(sem);
                if (*turn == i) {
                    printf("Process %d (PID %d): Entering critical section.\n", i, getpid());
                    sleep(1);  // Simulate work
                    int local = *shared_counter;
                    local++;
                    *shared_counter = local;
                    printf("Process %d (PID %d): Counter = %d\n", i, getpid(), *shared_counter);

                    printf("Process %d (PID %d): Leaving critical section.\n", i, getpid());
                    (*turn)++;  // Allow next process
                    sem_post(sem);
                    break;
                }
                sem_post(sem);
                usleep(10000);  // Prevent busy waiting
            }
            exit(0);
        }
    }

    // Parent waits for all children
    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    // Cleanup
    sem_close(sem);
    sem_unlink(SEM_NAME);
    munmap(turn, sizeof(int));
    printf("Final Counter: %d\n", *shared_counter);

    printf("All processes completed in order.\n");
    return 0;
}
