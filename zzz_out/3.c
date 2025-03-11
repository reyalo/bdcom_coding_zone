#include <stdio.h>

int main() {
    char ch;

    // Display ASCII values for printable characters
    printf("ASCII values for characters A to Z:\n");
    for (ch = 'A'; ch <= 'Z'; ch++) {
        printf("Character: %c, ASCII: %d\n", ch, ch);
    }

    printf("\nASCII values for lowercase characters a to z:\n");
    for (ch = 'a'; ch <= 'z'; ch++) {
        printf("Character: %c, ASCII: %d\n", ch, ch);
    }

    printf("\nASCII values for digits 0 to 9:\n");
    for (ch = '0'; ch <= '9'; ch++) {
        printf("Character: %c, ASCII: %d\n", ch, ch);
    }

    printf("\nSpecial characters and their ASCII values:\n");
    printf("Character: '\\a' (Bell), ASCII: %d\n", '\a');
    printf("Character: '\\n' (Newline), ASCII: %d\n", '\n');
    printf("Character: '\\t' (Tab), ASCII: %d\n", '\t');
    printf("Character: '\\v' (Vertical Tab), ASCII: %d\n", '\v');
    printf("Character: '\\f' (Form Feed), ASCII: %d\n", '\f');
    printf("Character: '\\r' (Carriage Return), ASCII: %d\n", '\r');
    printf("Character: '\\b' (Backspace), ASCII: %d\n", '\b');
    printf("Character: '\\0' (Null), ASCII: %d\n", '\0');
    printf("Character: '\\'' (Single Quote), ASCII: %d\n", '\'');
    printf("Character: '\\\"' (Double Quote), ASCII: %d\n", '\"');
    printf("Character: '\\\\' (Backslash), ASCII: %d\n", '\\');

    return 0;
}
