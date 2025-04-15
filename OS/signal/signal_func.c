#include <stdio.h>
#include <signal.h>

void handle_sigint(int sig) {
    printf("Caught signal %d (Ctrl+C). You can't stop me that easily!\n", sig);
}

int main() {
    signal(SIGINT, handle_sigint);  // set handler for SIGINT

    while (1) {
        printf("Running... Press Ctrl+C to try to interrupt.\n");
        sleep(1);
    }

    return 0;
}
