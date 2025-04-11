#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

int shared_counter = 0;
int turn = 1;  // Start with thread 1

// int t = 6;

sem_t mutex;

void* thread_func(void* arg) {
    int thread_id = *(int*)arg;

    while (1) {
        sem_wait(&mutex);
        if (turn == thread_id) {
            turn++;  // Allow next thread
            // Critical section
            printf("Thread %d: Entered critical section.\n", thread_id);

            sleep(rand()%6);
            int local_var = shared_counter;
            sleep(rand()%4);
            local_var++;
            shared_counter = local_var;


            sleep(1); 
            printf("Thread %d: Leaving critical section. Counter = %d\n", thread_id, shared_counter);

            sem_post(&mutex);
            break;   // Exit the loop and finish thread
        }
        sem_post(&mutex);
        usleep(10000); // Sleep briefly to avoid busy waiting
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex);

    printf("Final counter: %d\n", shared_counter);
    return 0;
}
