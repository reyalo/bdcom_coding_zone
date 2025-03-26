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


void print_char(const char *str, size_t sz){
    for(int i = 0; i < sz; i++){
        printf("%u ", str[i]);
    }
    printf("\n");
}

void print_charc(const char *str, size_t sz){
    for(int i = 0; i < sz; i++){
        printf("%c ", str[i]);
    }
    printf("\n");
}


int main() {
    
    char str1[] = "hello";
    char str2[] = "world_bangladesh";

    printf("%s\n", str2);
    

    memcpy(str1, str2, sizeof(str2));

    print_char(str1, sizeof(str2));

    printf("%s\n", str1);
    printf("%s\n", str2);


    return 0;
};