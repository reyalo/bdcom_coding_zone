#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

volatile sig_atomic_t woke_up_early = 0;

// Called when SIGALRM is received
void alarm_handler(int signum) {
    woke_up_early = 1;
}

// Simulate sleep using alarm + pause
unsigned int my_sleep(unsigned int seconds) {
    struct sigaction sa, old_sa;
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Set custom handler
    sigaction(SIGALRM, &sa, &old_sa);

    woke_up_early = 0;
    alarm(seconds);      // Set alarm
    pause();             // Wait until SIGALRM is caught
    alarm(0);            // Cancel any pending alarms

    // Restore old handler
    sigaction(SIGALRM, &old_sa, NULL);

    return woke_up_early ? 1 : 0;
}

// Periodic timer using alarm
void setup_periodic_timer(int interval_sec) {
    alarm(interval_sec);  // First fire
}

// Periodic handler
void periodic_handler(int signum) {
    printf("[TIMER] Waking process early at %ld\n", time(NULL));
    woke_up_early = 1;
    alarm(5);  // Schedule next periodic wake-up (e.g., every 5 sec)
}

int main() {
    // Install periodic timer handler
    signal(SIGALRM, periodic_handler);
    setup_periodic_timer(5);  // First timer fires after 5 seconds

    printf("Sleeping for 10 seconds (but may be interrupted early)...\n");

    // Simulated sleep (will be interrupted by periodic timer)
    int result = my_sleep(10);

    if (result)
        printf("Sleep was interrupted early by timer!\n");
    else
        printf("Slept full 10 seconds without interruption.\n");

    return 0;
}
