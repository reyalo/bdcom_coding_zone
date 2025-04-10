#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipefd[2];
    pipe(pipefd);

    char buffer[64];
    
    if (fork() == 0) {
        // Child process
        // char buffer[64];
        // read(pipefd[0], buffer, sizeof(buffer));  // May block indefinitely
    } else {
        // Parent process
        write(pipefd[1], "Hello", 5);
        printf("Parent: wrote to pipe\n");
        close(pipefd[1]);  // If not closed, child won't detect EOF

        read(pipefd[0], buffer, sizeof(buffer));  // May block indefinitely
        printf("Parent: read from pipe: %s\n", buffer);
    }

    return 0;
}