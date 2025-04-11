

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();

    if (pid == 0) {
        // Child
        // close(pipefd[1]); // ✅ Required to see EOF
        char buffer[100];
        int n;
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, n);
        }
        printf("Child: got EOF\n");
    }
    else {
        // Parent
        write(pipefd[1], "Hello\n", 6);
        sleep(2);
        // ❗ if you don't do this, child never gets EOF
        close(pipefd[1]); 
        wait(NULL);
    }

    return 0;
}
