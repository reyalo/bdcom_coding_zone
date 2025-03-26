#include <stdio.h>
#include <unistd.h> // For sleep()

int main() {
    printf("This will be printed... ");
    sleep(2); // Wait for 2 seconds
    printf("NOW!\n"); // Will not print immediately unless we flush
    
    fflush(stdout); // Flush stdout manually to force printing
    
    fprintf(stderr, "Error message appears immediately!\n");
    
    return 0;
}
