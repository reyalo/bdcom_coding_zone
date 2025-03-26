#include<stdio.h>

int main(void)
{
    int ints[20] = { 
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 
        110, 120, 130, 140, 150, 160, 170, 180, 190, 200 
        }; 

        int *ip = ints + 3;

    printf("%u\n", ints);
    printf("%u\n",  &ints + 4);
    printf("%u\n",  ((int)(&ints + 4) - (int)ints));

    printf("%x\n", &ints[1]);
    printf("%x\n", &ip[-2]);
    return 0;
}