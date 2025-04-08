#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to demonstrate the creation of a process using fork()
void create_process_fork() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child process (fork) created. PID = %d\n", getpid());
        exit(0);
    } else {
        printf("Parent process (fork). Child PID = %d\n", pid);
    }
}

// Function to demonstrate the creation of a process using vfork()
void create_process_vfork() {
    pid_t pid = vfork();
    if (pid < 0) {
        perror("vfork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child process (vfork) created. PID = %d\n", getpid());
        _exit(0);
    } else {
        printf("Parent process (vfork). Child PID = %d\n", pid);
    }
}

// Function to demonstrate the creation of a process using clone() (Linux-specific)
#define _GNU_SOURCE
#include <sched.h>

int child_func(void *arg) {
    printf("Child process (clone) created. PID = %d\n", getpid());
    return 0;
}

void create_process_clone() {
    char *stack = malloc(1024 * 1024);
    if (!stack) {
        perror("malloc failed");
        exit(1);
    }

    pid_t pid = clone(child_func, stack + 1024*1024, SIGCHLD, NULL);
    if (pid == -1) {
        perror("clone failed");
    } else {
        printf("Parent process (clone). Child PID = %d\n", pid);
    }

    free(stack);
}

// Function to suspend a process using SIGSTOP
void suspend_process(pid_t pid) {
    printf("Sending SIGSTOP to process with PID %d\n", pid);
    kill(pid, SIGSTOP);
    sleep(2);
    printf("Sending SIGCONT to process with PID %d\n", pid);
    kill(pid, SIGCONT);
}

// Function to terminate a process using exit() (from the child)
void terminate_process_exit() {
    printf("Child process terminating using exit()\n");
    exit(0);
}

// Function to terminate a process using kill() from parent
void terminate_process_kill(pid_t pid) {
    printf("Parent sending SIGKILL to child process PID = %d\n", pid);
    kill(pid, SIGKILL);
}

int main() {
    pid_t pid;

    // Create a process using fork
    printf("Creating process using fork...\n");
    create_process_fork();
    
    // Create a process using vfork
    printf("\nCreating process using vfork...\n");
    create_process_vfork();
    
    // Create a process using clone (Linux-specific)
    printf("\nCreating process using clone...\n");
    create_process_clone();

    // Create a process using fork for suspend/terminate demonstration
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        printf("\nChild process. Suspended and then terminated.\n");
        sleep(2);
        terminate_process_exit();
    } else {
        // Parent process
        sleep(1);  // Give child some time to run
        suspend_process(pid);  // Suspend the child
        sleep(2);
        terminate_process_kill(pid);  // Kill the child
        wait(NULL);  // Wait for child termination
    }

    return 0;
}
