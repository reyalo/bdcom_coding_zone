#include <stdio.h>

int main() {
    FILE *file = fopen("dummy.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 1; i <= 10000; i++) {
        fprintf(file, "%d\n", i);
    }

    fclose(file);
    printf("Numbers written to dummy.txt successfully.\n");
    return 0;
}