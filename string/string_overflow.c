#include<stdio.h>
#include<string.h>

int main(){
    char str[4] = "Hello world";

    printf("%d \n", str[4]);
    printf("Before null terminator : %s \n", str);
    str[3] = '\0';
    printf("After null terminator : %s \n", str);


    //_________test2____________
    printf("\n\n\n");

    char x = 'A';
    char message[10] = "messagewe"; 
    strcpy( message, "A different message_end" ); // must use NULL terminator in message[9]
    // strncpy( message, "A different message_end", 9 );  // solution

    printf(" %s \n", message);
    printf("%c\n", x);


    printf("%d \n", sizeof(message));   // 10
    printf("%d \n", strlen(message));   // 23


    //_________________test3_______________
    char message2[10] = "messagewe";
    char message3[50] = " A different message_end A different message_end";
    strcat(message2, message3);

    printf("%s\n", message2);
    printf("%s\n", message3);

    // strchr  -> if not match , return NULL pointer.
    // strrchr -> jodi kuno string match na korte pare tobe sobar last er ta te point kore thake
    
}