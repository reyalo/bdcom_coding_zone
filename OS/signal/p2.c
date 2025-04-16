#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf jump_buffer;

void signal_handler(int sig) {
    printf("Signal caught: %d\n", sig);
    longjmp(jump_buffer, 1);  // simulate a 'throw'
}

int main() {
    // Register signal handler (simulate exception source)
    signal(SIGFPE, signal_handler);  // for divide by zero
    signal(SIGSEGV, signal_handler); // for segmentation fault

    if (setjmp(jump_buffer) == 0) {
        // Try block
        printf("In try block\n");

        int a = 5;
        int b = 0;
        int c = a / b; // This will raise SIGFPE (divide by zero)

        printf("Result: %d\n", c); // Won't be reached
    } else {
        // Catch block
        printf("In catch block: exception handled\n");
    }

    // Finally block
    printf("In finally block: cleaning up\n");

    return 0;
}
