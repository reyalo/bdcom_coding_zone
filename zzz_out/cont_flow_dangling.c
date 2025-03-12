#include <stdio.h>

int main() {
    int x = 5, y = 10;

    if (x < 3) 
        if (y > 5) 
            printf("Both conditions are true\n");
    else 
        printf("Else block\n"); 
    // You might think the else belongs to the first if (x > 3). 
    // But in reality, it gets associated with the closest unmatched if, which
    // is if (y > 5).

    return 0;
}
