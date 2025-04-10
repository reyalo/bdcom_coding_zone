#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 100000

int counter = 0;
sem_t mutex;  // Semaphore for mutual exclusion

void* increment_counter(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sem_wait(&mutex);     // Wait (lock)
        counter++;            // Critical section
        sem_post(&mutex);     // Signal (unlock)
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize semaphore with 1 (acts like a mutex)
    sem_init(&mutex, 0, 1);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_counter, NULL);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphore
    sem_destroy(&mutex);

    // Final value of counter
    printf("Final counter value: %d (Expected: %d)\n", counter, NUM_THREADS * NUM_ITERATIONS);

    return 0;
}
