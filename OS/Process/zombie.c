#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    // pid_t pid2 = fork();
    // pid_t pid3 = fork();

    // if(pid > 0){
    //     printf("Parent Process: PID = %d\n", getpid());
    //     sleep(20);
    // } else if(pid == 0){
    //     printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
    //     sleep(20);
    //     exit(0); // Exiting child process
    // } else {
    //     printf("Fork failed!\n");
    // }

    if (pid > 0) {
        // Parent process
        printf("Parent process sleeping... PID = %d\n", getpid());
        // sleep(60);                   // Give time to observe the zombie
        // sleep(10);
        exit(0);                        // Parent exits immediately, child becomes orphan
    } else if (pid == 0) {
        // Child process
        sleep(30);
        printf("Child process exiting... PID = %d\n", getpid());
        // exit(0); // Exits, becomes zombie until parent collects it
        // sleep(10);
        // exit(0);
    } else {
        perror("fork");
    }

    return 0;
}
