#include <stdio.h>
#include <string.h>

void print_char(const char *str, size_t sz);
void print_charc(const char *str, size_t sz);




int main(void){

    freopen("output.txt", "w", stdout);

    unsigned char a = -1;
    unsigned short b = -1;
    unsigned int c = -1;

    printf("%d, %d, %d\n", a, a, a);
    // printf("%u, %u, %u\n", a, b, c);
    // printf("%hd, %hd, %hd\n", a, b, c);
    // printf("%hhd, %hhd, %hhd\n", a, b, c);

}

/*

1.

    char str5[] = "Hello";
    char str6[] = "Hello World";
    strcpy(str5, str6);

    printf("%p\n", &str5);  // 0x7ffd14eff0e6
    printf("%p\n", &str6);  // 0x7ffd14eff0ec

    puts(str5);             // Hello World (Undefined Behavior)
    puts(str6);             // World       (Undefined Behavior)


2.
    char str3[] = {'J', 'I', 'M'};
    char str4[] = "JIM";

    printf("%d\n", strcmp(str3, str4));         // result >= 0 (Undefined Behavior)
    printf("%d\n", strncmp(str4, str3, 3));     // result 0


3.

    char str1[] = "Hello";                    // if comment out this line,
    char str2[] = {'H', 'E', 'L', 'L', 'O'};     // strlen(str2 will be 10)
    printf("%lu\n", strlen(str2));               // min(5, ram_size) undefined behavior
    printf("%lu\n", sizeof(str2));               // 5

4.
    char str7[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};      // not null terminated
    char str8[] = "Hello";
    strncpy(str7, str8, 10);        // char str7[10] = {'H', 'e', 'l', 'l', 'o', '\0', '0\', '\0', '\0', '\0'};
    puts(str7);


5.
har *str1 = "A";
char *str2 = "ABC";
char *str3 = "ABCDEFGH";

printf("start:%5s, %.5s, %5.6s\n", str1, str1, str1);       // start: ****A   , A    , ****A
printf("start:%5s, %.5s, %5.6s\n", str2, str2, str2);       // start: **ABC   , ABC  , **ABC
printf("start:%5s, %.5s, %5.6s\n", str3, str3, str3);       // start: ABCDEFGH, ABCDE, ABCDEF

6.
char str13[11] = "Hello";
    char str14[3] = {'J', 'I', 'M'};    // str14 = not null terminated
    strcat(str13, str14);               // undefined behavior
    puts(str13);                        // strcat(dst, src){
                                    ;   //     strcpy(dst + strlen(dst), src);
                                    ;   //     return dst;


7.
unsigned char a = -1;
unsigned short b = -1;
unsigned int c = -1;

printf("%d, %d, %d\n", a, b, c);
printf("%u, %u, %u\n", a, b, c);
printf("%hd, %hd, %hd\n", a, b, c);
printf("%hhd, %hhd, %hhd\n", a, b, c);


8.
unsigned char a = 0x33, b = 0x55;
printf("%d %d %d %d\n",~a, !b, a & b, a || b, a ^ b);


9.
char a = 150, b = 50;
unsigned char c = 200, d = -60;
unsigned char e = 200, f = 150;

printf("%d %d %d\n", a + b, c - d, c + e - f);


10.
length = 0;

unsigned int sum_elements(unsigned int a[], unsigned int length){
    char i;
    unsigned char result = 0;

    for(i = 0; i < length; i++){
        result += a[i];
    }
    return result;
}



*/

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
