#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {
    int client_fd;
    struct sockaddr_un addr;

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/mysocket");
    connect(client_fd, (struct sockaddr*)&addr, sizeof(addr));

    write(client_fd, "Hello, server!", 14);
    close(client_fd);
    return 0;
}
