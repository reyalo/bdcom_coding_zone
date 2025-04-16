#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>

#define FILE_ERROR   1
#define SEG_FAULT    2

jmp_buf g_ev;

void file_operation(void)
{
    FILE *file = NULL;
    
    if ((file = fopen("tedst.txt", "r")) == NULL)
    {
        printf("Simulating file handling error.\n");
        fclose(file);
        longjmp(g_ev, FILE_ERROR);
    }
    
    fclose(file);
    return;
}

void signal_handler(int sig)
{
    printf("Simulating segment fault error.\n");
    longjmp(g_ev, SEG_FAULT);
    
    return;
}

int main(void)
{   
    signal(SIGSEGV, signal_handler);  // Catch the segment fault error

    switch(setjmp(g_ev))
    {
        case 0:
            printf("Try Simulation\n");        
            file_operation();
            
            {
                int *p = NULL;
                printf("Accessing null pointer...\n");
                *p = 1;                
            }
        
        break;
        case FILE_ERROR:
            printf("Catch Simulation, file exception Handled.\n");
        break;
        case SEG_FAULT:
            printf("Catch Simulation, segment fault Handled.\n");
        break;
        default:
            printf("Other exception Handled.\n");
        break;
    }

    printf("Finally Simulation.\n");

    return 0;
}
