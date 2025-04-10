#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

// Semaphore for mutual exclusion
sem_t mutex;

// Shared resource
int shared_counter = 0;
int t = 5;

// Thread function
void* thread_func(void* arg) {
    int thread_id = *(int*)arg;
    sleep(t--);

    
    printf("Thread %d: Waiting to enter critical section...\n", thread_id);
    
    sem_wait(&mutex);
    int local = shared_counter;
    
    printf("Thread %d: Entered critical section.\n", thread_id);
    // sleep(1);
    sleep(rand() % 3);
    local++;

    shared_counter = local;
    printf("Thread %d: Leaving critical section. Counter = %d\n", thread_id, shared_counter);
    sem_post(&mutex);


    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Initialize semaphore with value 1 (binary semaphore = mutex)
    sem_init(&mutex, 0, 1);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphore
    sem_destroy(&mutex);

    printf("Final Counter: %d\n", shared_counter);
    return 0;
}


/*
Implementation:
int pthread_create(
    pthread_t *thread,            // Output: ID of the new thread
    const pthread_attr_t *attr,   // Thread attributes (can be NULL for default)
    void *(*start_routine)(void*),// Pointer to the function that function receive a void pointer and return a void pointer
    void *arg                     // Argument passed to the function
);


*/
