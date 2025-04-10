#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 4

sem_t sems[NUM_THREADS];  // One semaphore per thread

void* thread_func(void* arg) {
    int id = *(int*)arg;

    // Wait for our turn
    sem_wait(&sems[id]);

    // Critical section
    printf("Thread %d: Entering critical section\n", id + 1);
    sleep(1); // Simulate work
    printf("Thread %d: Leaving critical section\n", id + 1);

    // Signal the next thread, if any
    if (id + 1 < NUM_THREADS) {
        sem_post(&sems[id + 1]);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    // Initialize semaphores
    for (int i = 0; i < NUM_THREADS; i++) {
        sem_init(&sems[i], 0, (i == 0) ? 1 : 0); // First one unlocked, rest locked
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_func, &ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Clean up
    for (int i = 0; i < NUM_THREADS; i++) {
        sem_destroy(&sems[i]);
    }

    printf("All threads completed in order.\n");
    return 0;
}
