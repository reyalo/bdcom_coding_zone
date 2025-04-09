#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char *fifo = "/tmp/myfifo";

    // Open FIFO for write only
    fd = open(fifo, O_WRONLY);

    // Write a message
    write(fd, "Hello from writer process!\n", 27);

    close(fd);
    return 0;
}
