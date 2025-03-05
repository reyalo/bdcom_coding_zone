#include <stdio.h>

void function(){

    for(int i = 0; i < 10; ++i){
        for(int j = 0; j < 10; ++j){
            if(i+j <= 0){
                return; // return from here and function and loop execution stop
            }
        }
    }
    printf("Left part of this code\n");
}

int main() {

    // break
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            if (j == 2) break;  // Exits inner loop when j == 2
            printf("i = %d, j = %d\n", i, j);
        }
    }

    // continue
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            if (j == 2) continue;;  // Skip left of the code of inner loop when j == 2
            printf("i = %d, j = %d\n", i, j);
        }
    }
    function();
    return 0;
}
