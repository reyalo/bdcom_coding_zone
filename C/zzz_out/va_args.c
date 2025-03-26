#include <stdio.h>
#define debug(fmt, ...) printf(fmt" This flag will show last\n", ##__VA_ARGS__)

int main() {
    debug("Hello, world!\n");
    debug("Value: %d %d\n", 42, 44);
    return 0;
}
