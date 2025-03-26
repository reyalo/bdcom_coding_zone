#include <stdio.h>
#include <pthread.h>

void* threadFunction1(void* arg) {
    printf("Hello from the thread1!\n");
    return NULL;
}

void* threadFunction2(void* agr){
    printf("Hello from the thread2!\n");
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, threadFunction1, NULL);      // create a thread
    pthread_create(&thread2, NULL, threadFunction2, NULL);      
    // pthread_join(thread1, NULL);     // pause the main program until the thread1 and thread2 finished the programm
    // pthread_join(thread2, NULL);

    printf("\n\nThis line always execute after the execution of thread\n\n");

    return 0;
}
