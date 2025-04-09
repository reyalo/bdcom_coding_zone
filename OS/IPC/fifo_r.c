#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char *fifo = "/tmp/myfifo";
    char buffer[100];

    // Open FIFO for read only
    fd = open(fifo, O_RDONLY);

    // Read message from FIFO
    read(fd, buffer, sizeof(buffer));
    printf("Received: %s", buffer);

    close(fd);
    return 0;
}
