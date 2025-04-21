#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

char* safe_ctime(const time_t* t, char* buf, size_t buflen)
{
    sigset_t newmask, oldmask;
    sigfillset(&newmask);

    pthread_sigmask(SIG_BLOCK, &newmask, &oldmask);

    pthread_mutex_lock(&lock);

    char* result = ctime(t);
    if (result)
	{
        strncpy(buf, result, buflen - 1);
        buf[buflen - 1] = '\0';
    }
	
	// sleep(5);

    pthread_mutex_unlock(&lock);

    pthread_sigmask(SIG_SETMASK, &oldmask, NULL);

    return buf;
}

void handler(int sig) {
    printf("Thread %lu got signal %d\n", pthread_self(), sig);
    pthread_exit(NULL);  // Exit the thread gracefully
}

void* thread_func(void* arg) {

    sigset_t newmask1, oldmask1;
    sigfillset(&newmask1);

    pthread_sigmask(SIG_BLOCK, &newmask1, &oldmask1);

    signal(SIGUSR1, handler);
 

    time_t now = time(NULL);
    char buf[64];

    printf("pthread_self: %lu\n", pthread_self());

    sleep(5);

    safe_ctime(&now, buf, sizeof(buf));
    printf("Thread %lu: %s", (long)arg, buf);


    // pthread_sigmask(SIG_BLOCK, &newmask1, &oldmask1);

    return NULL;
}

int main() {

    sigset_t newmask1, oldmask1;
    sigfillset(&newmask1);

    pthread_sigmask(SIG_BLOCK, &newmask1, &oldmask1);
	
    pthread_t t1, t2, t3;
    // return 0;
    pthread_create(&t1, NULL, thread_func, (void*)1);
    pthread_create(&t2, NULL, thread_func, (void*)2);

    sleep(2);

    printf("Main: sending SIGUSR1 to thread %lu\n", t1);
    pthread_kill(t1, SIGUSR1);  // Send signal to specific thread

    // sleep(5); // Let signal be handled

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_sigmask(SIG_BLOCK, &newmask1, &oldmask1);


    return 0;
}