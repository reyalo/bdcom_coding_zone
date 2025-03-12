#include <stdio.h>
#include <string.h>

void print_c(char *str) {
    for (int i = 0; i < 40; i++) {
        printf("str[%d]: %c\n", i, str[i]);
    }
}
void print_n(char *str) {
    for (int i = 0; i < 40; i++) {
        printf("str[%d]: %d\n", i, str[i]);
    }
}


int main() {
    
    char str1[] = "hello";
    char str2[] = "world_bangladesh";

    




    return 0;
};