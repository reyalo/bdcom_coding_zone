#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int sig) {
    printf("Thread %lu got signal %d\n", pthread_self(), sig);
    pthread_exit(NULL);  // Exit the thread gracefully
}

void* thread_func(void* arg) {
    signal(SIGUSR1, handler);  // Setup signal handler for this thread

    while (1) {
        printf("Thread %lu running...\n", pthread_self());
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    sleep(3);  // Let thread run a bit

    printf("Main: sending SIGUSR1 to thread %lu\n", tid);
    pthread_kill(tid, SIGUSR1);  // Send signal to specific thread

    pthread_join(tid, NULL);
    printf("Main: thread exited.\n");

    return 0;
}
