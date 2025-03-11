#include <stdio.h>
#include <stdbool.h>

static int times_eaten = 0;

// Example function declarations
void eat_hamburger() {
    printf("Eating a hamburger...\n");
    times_eaten++;
}

bool hunger() {

    // Let's assume after 3 hamburgers, you're no longer hungry.
    return times_eaten <= 3; // Return true if you're still hungry
}

int main() {
    // Eat hamburgers as long as you're hungry
    while (hunger()) {
        eat_hamburger();
    }

    printf("I'm no longer hungry!\n");
    return 0;
}
