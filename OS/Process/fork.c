#include<stdio.h>

static int cnt=0;

int main(){

    fork();
    //sleep(15);
    fork();
    //sleep(7);
    fork();
    
    
    printf("PID : %d PPID : %d\n", getpid(), getppid());
    sleep(10);

    

}