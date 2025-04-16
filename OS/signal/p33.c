#include <stdio.h>
#include <unistd.h>
#include <signal.h>


#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))


volatile sig_atomic_t time_rem = 0;
int period = 6; // Period for the alarm in seconds


void alarm_handler(int signum) {
    alarm(period);
    
}

void my_sleep(int seconds) {
    time_rem = seconds;

    signal(SIGALRM, alarm_handler);


    alarm( min(time_rem, 6));               // Set the first alarm for the smaller of time_rem time or 6 seconds

    while (time_rem > 0){
        pause();                            // Wait for next alarm signal

        period = min(time_rem, period);     // Update the period for the next alarm
        time_rem -= period;                 // Decrease the remaining time by the period

        printf("Alarm! Wake up after %d seconds... Remaining: %d seconds\n",period, time_rem);
    }
}

int main() {
    printf("Sleeping for 20 seconds with periodic alarm every 6 seconds...\n");
    my_sleep(20);
    printf("Done sleeping!\n");

    return 0;
}
