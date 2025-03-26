#include <stdio.h>
#include <string.h>


struct Point {
    int x;
    int y;
};

int main() {
    // 1. Designated Initializers (C99+)
    struct Point p1 = {.x = 10, .y = 20}; // Designated Initializers (C99+)

    // 2. Braced List Initialization (Traditional)
    struct Point p2 = {10, 20};  // Ordered Initialization

    // 3. Partial Initialization
    struct Point p3 = {10};  // Only x is initialized, y is set to 0

    // 4. Using memset()
    struct Point p4;
    memset(&p4, 0, sizeof(p4));  // Set all members to 0
    p4.x = 5; 
    p4.y = 15;

    // 5. Using malloc() (Dynamic Memory Allocation)
    struct Point *p5 = (struct Point *)malloc(sizeof(struct Point));
    p5->x = 12;
    p5->y = 24;

    // 6. Using Compound Literals (C99 and later)
    struct Point p6 = (struct Point){30, 40};  // Temporary struct object



    // printf("p1: x = %d, y = %d\n", p1.x, p1.y);
    return 0;
}
