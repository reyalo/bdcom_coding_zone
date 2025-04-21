#include<stdio.h>

int main(){
    int cnt = 10;
    // fork();
    if(0){
        printf("child\n");
        // while (cnt--){
        //     sleep(1);
        //     printf("child\n");
        // }
        
    }
    else {
        printf("parent\n");
        while (cnt-- > 0){
            sleep(1);
            printf("child\n");
        }
    }
    
    
    return 0;
}