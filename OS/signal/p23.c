#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>

jmp_buf jump_buf;
int exception_code = 0;

#define TRY     if ((exception_code = setjmp(jump_buf)) == 0)
#define CATCH   else
#define FINALLY if (1)
#define THROW(code) longjmp(jump_buf, code)

#define FILE_NOT_OPENED 1001

void signal_handler(int signum) {
    THROW(signum);  // Using the macro instead of calling longjmp directly
}

int main() {
    // Set up signal handlers
    signal(SIGSEGV, signal_handler);  // Invalid memory access

    FILE *file = NULL;

    TRY {
        printf("In TRY block\n");

        // Try to open a file
        file = fopen("non_existing_file.txt", "r");
        if (file == NULL) {
            printf("File cannot be opened, throwing exception...\n");
            THROW(FILE_NOT_OPENED);  // 1 = File open error
        }

        printf("File opened successfully\n");
        fclose(file);

        // Uncomment to simulate divide by zero:
        // int x = 5 / 0;
    }
    CATCH {
        if (exception_code == FILE_NOT_OPENED) {
            printf("Caught exception: File open error\n");
        } else if (exception_code == SIGSEGV) {
            printf("Caught signal: Segmentation fault\n");
        } else {
            printf("Caught unknown exception (code: %d)\n", exception_code);
        }
    }
    FINALLY {
        printf("In FINALLY block — this always runs\n");
    }

    return 0;
}
