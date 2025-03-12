#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global String Declarations
// 1.Global Character Array (Stored in Data Section)
char str8[] = "Hello World!";

// 2.Global Constant Character Array (Stored in Read-Only Data Section)           
const char str9[] = "Hello World!";

// 3.Global Static Character Array (Stored in Data Section)      
static char str10[] = "Hello World!";

int main() {
    // 4. String Literal (Stored in Read-Only Data Section)
    char *str1 = "Hello World!";


    // 5. Character Array (Stored in Stack)
    char str2[] = "Hello World!";


    // 6. Explicitly Initialized Character Array (Stored in Stack)
    char str3[13] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\0'};


    // 7. Dynamically Allocated String (Stored in Heap)
    char *str4 = (char *)malloc(13);
    strcpy(str4, "Hello World!");


    // 8. Constant Character Array (Stored in Read-Only Data Section)
    const char str5[] = "Hello World!";


    // 9. Static Character Array (Stored in Data Section)
    static char str6[] = "Hello World!";
;

    // 10. Static Pointer to String Literal (Stored in Read-Only Data Section)
    static char *str7 = "Hello World!";


    // 11. Volatile Character Array (Stored in Stack)
    volatile char str11[] = "Hello World!";

    // 12. Using strdup() (Stored in Heap)
    char *str12 = strdup("Hello World!");

    return 0;
}
