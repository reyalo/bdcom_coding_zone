#include <stdio.h>

volatile int flag = 0; // flag can be modified by an external event (e.g., interrupt)

void interrupt_service_routine() {
    flag = 1; // Assume an interrupt changes flag
}

int main() {
    while (flag == 0) {
        // Loop runs indefinitely if the compiler optimizes the flag check
        // But volatile prevents such optimization
    }
    printf("Flag changed!\n");
    return 0;
}
