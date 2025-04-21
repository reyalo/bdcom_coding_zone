#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <string.h>

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
	
	//sleep(5);

    pthread_mutex_unlock(&lock);

    pthread_sigmask(SIG_SETMASK, &oldmask, NULL);

    return buf;
}

void* thread_func(void* arg) {
    time_t now = time(NULL);
    char buf[64];
	
	printf("pthread_self: %lu\n", pthread_self());

    safe_ctime(&now, buf, sizeof(buf));
    printf("Thread %lu: %s", (long)arg, buf);

    return NULL;
}

int main() {
	
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, thread_func, (void*)1);
    pthread_create(&t2, NULL, thread_func, (void*)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}