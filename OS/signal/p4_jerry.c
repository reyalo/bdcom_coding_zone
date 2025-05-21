
#include <signal.h>
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h>

#define NUM 10
sem_t sem_1, sem_2;

void *start_routine_1(void *args)
{
    int i;
    for (i = 0; i < NUM; i++)
    {
        sem_wait(&sem_1); sleep (3);
        printf("%d Hello ", i + 1); 
        sem_post(&sem_2);
    }
    return NULL;
}

void *start_routine_2 (void *args)
{
    int i;

    for (i = 0; i < NUM; i++)
    {
        sem_wait(&sem_2);
        printf("BDCOM!\n");
        sem_post(&sem_1);
    }

    return NULL;
}

void handler (int sig)
{
    sem_wait(&sem_1); sleep (10);
    sem_post(&sem_1);
    return;
}

int main(void)
{
    pthread_t tid_1, tid_2;

    if (sem_init(&sem_1, 0, 1) == -1)
        printf("sem_init");
    if (sem_init(&sem_2, 0, 0) == -1)
        printf("sem_init");

    signal (SIGINT, handler);

    pthread_create(&tid_1, NULL, start_routine_1, NULL);
    pthread_create(&tid_2, NULL, start_routine_2, NULL);

    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);

    return 0;
}

/*

1 Hello BDCOM!
2 Hello BDCOM!
^C               ← Ctrl+C pressed here
my answer is deadlock here if ctrl+c pressed when printf() is running
in thread 1
⏳ [10s delay due to handler()]
3 Hello BDCOM!


*/