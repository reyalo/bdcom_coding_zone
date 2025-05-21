#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 100

static int g_data = 0;
static sem_t sem_hello, sem_bdcom;
static pthread_mutex_t mu_lock = PTHREAD_MUTEX_INITIALIZER;

void *start_routine_hello(void *args)
{
	int i;
	
	for (i = 0; i < NUM; i++)
	{
		sem_wait(&sem_bdcom);
		
		pthread_mutex_lock(&mu_lock);
		g_data++;
		pthread_mutex_unlock(&mu_lock);
		
		printf("Hello(%d) ", g_data);
		sem_post(&sem_hello);
	}
	
	return NULL;
}

void *start_routine_bdcom(void *args)
{
	int i;
	
	for (i = 0; i < NUM; i++)
	{
		sem_wait(&sem_hello);
		
		pthread_mutex_lock(&mu_lock);
		g_data--;
		pthread_mutex_unlock(&mu_lock);
		
		printf("BDCOM(%d)\n", g_data);
		
		sem_post(&sem_bdcom);
	}
	
	return NULL;
}

int main(void)
{
	pthread_t tid_hello, tid_bdcom;
	
	if (sem_init(&sem_hello, 0, 0) == -1)
		printf("sem_init");
	
	if (sem_init(&sem_bdcom, 0, 1) == -1)
		printf("sem_init");
	
	pthread_create(&tid_hello, NULL, start_routine_hello, NULL);
	pthread_create(&tid_bdcom, NULL, start_routine_bdcom, NULL);
	pthread_join(tid_hello, NULL);
	pthread_join(tid_bdcom, NULL);
	
	printf("g_data=%d\n", g_data);
	
    return 0;
}
