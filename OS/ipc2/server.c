#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {
    int server_fd;
    struct sockaddr_un addr;
    char buf[100];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/mysocket");
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    int client_fd = accept(server_fd, NULL, NULL);
    
    read(client_fd, buf, sizeof(buf));
    printf("Received: %s\n", buf);

    close(client_fd);
    close(server_fd);
    unlink("/tmp/mysocket");
    return 0;
}
