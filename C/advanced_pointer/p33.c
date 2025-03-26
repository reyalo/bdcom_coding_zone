#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    
    // Method 1: Array notation
    printf("Method 1:\n");
    for(int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    // Method 2: Pointer arithmetic
    printf("\nMethod 2:\n");
    char **arg = argv;
    while(*arg) {
        printf("%s\n", *arg++);
    }


    return 0;
}