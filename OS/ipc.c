#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int ptc[2], ctp[2]; // Two pipes
    pipe(ptc); // Pipe 1: Parent → Child
    pipe(ctp); // Pipe 2: Child → Parent

    pid_t pid = fork(); // Fork a child process

    if (pid > 0) { // Parent Process
        close(ptc[0]); // Close read end of Pipe 1 (Parent → Child)
        close(ctp[1]); // Close write end of Pipe 2 (Child → Parent)

        char *messages[] = {"Hello, Child!", "How are you?", "Goodbye!"};
        char buffer[50];
        
        for (int i = 0; i < 3; i++) {
            write(ptc[1], messages[i], strlen(messages[i]) + 1); // Send message
            read(ctp[0], buffer, sizeof(buffer)); // Receive reply
            printf("Parent received: %s\n", buffer);
        }

        close(ptc[1]); // Close write end of Pipe 1
        close(ctp[0]); // Close read end of Pipe 2
    } 
    else if (pid == 0) { // Child Process
        close(ptc[1]); // Close write end of Pipe 1 (Parent → Child)
        close(ctp[0]); // Close read end of Pipe 2 (Child → Parent)

        char buffer[50];
        char *responses[] = {"Hi, Parent!", "I'm fine!", "See you!"};

        for (int i = 0; i < 3; i++) {
            read(ptc[0], buffer, sizeof(buffer)); // Read from parent
            printf("Child received: %s\n", buffer);
            write(ctp[1], responses[i], strlen(responses[i]) + 1); // Send reply
        }

        close(ptc[0]); // Close read end of Pipe 1
        close(ctp[1]); // Close write end of Pipe 2
    }

    return 0;
}
