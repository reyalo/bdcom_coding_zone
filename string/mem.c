#include<stdio.h>


int main(){
    int a, b, c, d;
    a = 1;
    b = -1;
    c = 2;
    d = -2;

    int ret = memcmp(&b, &b, sizeof(int));
    // ret = (unsigned char)a - (unsigned char)b;
    memset(&a, '0', 4);
    char *p = &a;
    printf("%x\n", p[1]);
    printf("%d\n", ret);
}