#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    char *arr[] = {"Hello!!!!!!!!!!!!!", "World", "C", NULL};
    char **ptr = arr;
    
    
    printf("%x %x %x\n",arr, &arr, &arr + 2);
    printf("%x %x %x\n",ptr, &ptr, ptr + 2);
}