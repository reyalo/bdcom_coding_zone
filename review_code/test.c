#include<stdio.h>



unsigned int sum_elements(unsigned int a[], unsigned int length){
    char i;
    unsigned char result = 0;

    for(i = 0; i <= length - 1; i++){
        result += a[i];
    }
    return result;
}

int main(){

    unsigned int a[400] = {1, 2, 3, 4};

    int ans = sum_elements(a, 0);

    printf("%d\n",ans);

}