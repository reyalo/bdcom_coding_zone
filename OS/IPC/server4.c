#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/unix_socket"
#define MAX_BUF 1024

#define TYPE_FILE_CONTENT 1
#define TYPE_ERROR_MSG    2

typedef struct {
    uint8_t type;
    uint32_t length;
    char value[];
} __attribute__((packed)) tlv_t;

void send_tlv(int fd, uint8_t type, const char *data, uint32_t len) {
    tlv_t *msg = malloc(sizeof(tlv_t) + len);
    msg->type = type;
    msg->length = len;
    memcpy(msg->value, data, len);
    write(fd, msg, sizeof(tlv_t) + len);
    free(msg);
}

void handle_client(int client_fd) {
    char filename[MAX_BUF] = {0};
    int n = read(client_fd, filename, MAX_BUF - 1);
    if (n <= 0) return;

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        char err[256];
        snprintf(err, sizeof(err), "Cannot open file '%s': %s", filename, strerror(errno));
        send_tlv(client_fd, TYPE_ERROR_MSG, err, strlen(err));
        return;
    }

    char buf[MAX_BUF];
    ssize_t bytes;
    while ((bytes = read(fd, buf, MAX_BUF)) > 0) {
        send_tlv(client_fd, TYPE_FILE_CONTENT, buf, bytes);
    }
    close(fd);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;

    unlink(SOCKET_PATH);

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("Server listening on %s\n", SOCKET_PATH);

    while ((client_fd = accept(server_fd, NULL, NULL)) != -1) {
        handle_client(client_fd);
        close(client_fd);
    }

    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
