#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[1024];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Child process
    if (pid == 0) {
        // Redirect stdout to pipe write-end
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); // Close read end in child
        close(pipefd[1]);

        // Execute ping
        execlp("ping", "ping", "127.0.0.1", "-c", "3", NULL);

        // If execlp fails
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(pipefd[1]); // Close write end in parent

        // Read output from pipe
        printf("Output of ping:\n");
        while (fgets(buffer, sizeof(buffer), fdopen(pipefd[0], "r")) != NULL) {
            printf("%s", buffer);
        }

        close(pipefd[0]);

        // Wait for child to finish
        wait(NULL);
    }

    return 0;
}
