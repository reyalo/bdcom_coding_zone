#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void alarm_handler_1(int signo) 
{
    printf("I'm wake up\n");
}

void alarm_handler_2(int signo) 
{
    printf("Alarm\n");
    kill(getppid(), SIGALRM);
    alarm(3);
}

int main(void) 
{
    int child_pid = fork();
    
    if (child_pid == -1) 
    {
        perror("fork");
        return 1;
    }

    
    if (child_pid == 0) 
    {
        signal(SIGALRM, alarm_handler_2); 
        alarm(3);
        
        while(1);
    }
    else
    {
        while(1)
        {
            printf("I'm sleeping\n");
            signal(SIGALRM, alarm_handler_1); 
            alarm(10);
            pause();
        }
        
        wait(NULL);
    }

    return 0;
}
