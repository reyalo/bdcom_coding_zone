#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

volatile sig_atomic_t sleep_done = 0;
volatile sig_atomic_t timer_interrupt = 0;

// Sleep signal handler
void sleep_handler(int signum) {
    sleep_done = 1;
}

// Periodic timer signal handler
void timer_handler(int signum) {
    timer_interrupt = 1;
    printf("[Timer] Periodic wake-up signal received.\n");
}

// Our custom sleep that can be interrupted early
void mysleep(int seconds) {
    sleep_done = 0;
    signal(SIGALRM, sleep_handler);

    alarm(seconds);

    while (!sleep_done) {
        pause();  // Wait for signal
        if (timer_interrupt) {
            printf("[Sleep] Interrupted by periodic timer!\n");
            break;
        }
    }

    alarm(0);  // Cancel remaining alarm if any
}

int main() {
    // Setup periodic timer every 3 seconds
    signal(SIGALRM, timer_handler);
    
    int timer_interval = 3;
    int next_alarm = timer_interval;

    time_t start = time(NULL);

    // Fork to run the periodic timer separately
    if (fork() == 0) {
        while (1) {
            sleep(timer_interval);
            kill(getppid(), SIGALRM);  // Send SIGALRM to parent
        }
        exit(0);
    }

    printf("Main process starting sleep for 10 seconds.\n");

    mysleep(10);  // Should be interrupted by the periodic timer

    time_t end = time(NULL);

    printf("Main process woke up after %ld seconds.\n", end - start);

    return 0;
}


