#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[128];
    // char buffer[1024];
    ssize_t bytesRead;

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process

        // Close read end of the pipe
        close(pipefd[0]);

        // Redirect stdout to write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO); // Redirect stderr too (optional)

        // Close original write end of the pipe
        close(pipefd[1]);

        // Execute ping command
        execlp("ping", "ping", "127.0.0.1", "-c", "3", (char *)NULL);

        // If execlp fails
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process

        // Close write end of the pipe
        close(pipefd[1]);

        // Read the output
        printf("Output of 'ping 127.0.0.1 -c 3':\n\n");
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {         // same as memcpy
            buffer[bytesRead] = '\0'; // Null-terminate buffer
            printf("%s", buffer);
        }

        // Close read end
        close(pipefd[0]);

        // Wait for child to finish
        wait(NULL);
    }

    return 0;
}
