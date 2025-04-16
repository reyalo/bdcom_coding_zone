#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>

jmp_buf jump_buf;
// int exception_code = 0;

void signal_handler(int signum) {
    // exception_code = signum;         // Save signal code
    longjmp(jump_buf, signum);            // Jump back to try block
}

int exception_code = 0; // Global variable to store exception code

#define TRY     if (( exception_code = setjmp(jump_buf)) == 0)
#define CATCH   else
#define FINALLY if (1)

int main() {
    // Set signal handlers
    signal(SIGFPE, signal_handler);   // Divide by zero
    signal(SIGSEGV, signal_handler);  // Invalid memory access

    TRY {
        printf("In TRY block\n");

        int a = 10;
        int b = 0;
        int c = a / b;                  // Causes SIGFPE (divide by zero)
        (void)c;

        printf("This line won't be reached on exception\n");
    }
    CATCH {
        printf("Caught signal %d: ", exception_code);
        if (exception_code == SIGFPE)
            printf("Divide by zero error\n");
        else if (exception_code == SIGSEGV)
            printf("Segmentation fault\n");
        else
            printf("Unknown error\n");
    }
    FINALLY {
        printf("In FINALLY block — this always runs\n");
    }

    return 0;
}
