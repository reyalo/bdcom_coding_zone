#include <stdio.h>
#include<string.h>



int main() {
    freopen("output.txt", "w", stdout);
    // char a = 2, b = -2;
    // unsigned char c = 2, d = -2;

    // printf("%x %x\n", a & 0xff, a & 0xffff);
    // printf("%x %x\n", b & 0xff, b & 0xffff);
    // printf("%x %x\n", c & 0xff, c & 0xffff);
    // printf("%x %x\n", d & 0xff, d & 0xffff);



    

    // char str1 = "JIM";
    // char str2 = {'J', 'I', 'M'};

    

    // printf("%d\n", strcmp(str2, str1));

    char *str = NULL;

    // int d = strlen(str);

    char str2[] = "ab\b\111";

    printf("%d\n", sizeof(str2));

    printf("\n\n%#-05o\n", 36);

}
