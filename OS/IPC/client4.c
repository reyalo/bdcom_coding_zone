#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdint.h>

#define SOCKET_PATH "/tmp/unix_socket"
#define MAX_BUF 1024

typedef struct {
    uint8_t type;
    uint32_t length;
    char value[];
} __attribute__((packed)) tlv_t;

ssize_t read_n_bytes(int fd, void *buf, size_t n) {
    size_t total = 0;
    ssize_t bytes;
    while (total < n) {
        bytes = read(fd, (char*)buf + total, n - total);
        if (bytes <= 0) return bytes; // error or EOF
        total += bytes;
    }
    return total;
}

void receive_tlv_stream(int sockfd) {
    while (1) {
        struct {
            uint8_t type;
            uint32_t length;
        } header;

        ssize_t hdr_bytes = read_n_bytes(sockfd, &header, sizeof(header));
        if (hdr_bytes <= 0) break;

        uint32_t len = header.length;
        char *value = malloc(len);
        if (!value) break;

        ssize_t val_bytes = read_n_bytes(sockfd, value, len);
        if (val_bytes != len) {
            free(value);
            break;
        }

        if (header.type == 1) {
            fwrite(value, 1, len, stdout);
        } else if (header.type == 2) {
            fprintf(stderr, "Error from server: %.*s\n", len, value);
        }

        free(value);
    }
}

int main() {
    int sockfd;
    struct sockaddr_un addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }

    char filename[256];
    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // remove newline

    write(sockfd, filename, strlen(filename) + 1);
    receive_tlv_stream(sockfd);

    close(sockfd);
    return 0;
}
