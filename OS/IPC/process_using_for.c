/*
This code demonstrates the use of fork() in a loop and what is value of i after each fork() call.

*/

#include<stdio.h>

static int cnt=0;

int main(){
    int i = 0;

    for(i = 0; i < 3; i++){
        printf("i = %d\n", i);
        fork();
        //sleep(15);
    }
    
    printf("i = %d\n", i);
    printf("PID : %d PPID : %d i = %d\n", getpid(), getppid(), i);
    sleep(1);

    

}