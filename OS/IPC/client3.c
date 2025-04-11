#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdint.h>

#define SOCKET_PATH "/tmp/unix_domain_socket"
#define MAX_BUF 2048

typedef struct {
    uint8_t type;
    uint32_t length;
    char value[]; // flexible array
} __attribute__((packed)) tlv_t;

int main() {
    int sockfd;
    struct sockaddr_un addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        exit(1);
    }

    char filename[256];
    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline

    write(sockfd, filename, strlen(filename) + 1); // send filename

    char buffer[MAX_BUF];
    ssize_t n;
    while ((n = read(sockfd, buffer, sizeof(buffer))) > 0) {
        tlv_t *msg = (tlv_t*)buffer;

        if (msg->type == 1) {
            fwrite(msg->value, 1, msg->length, stdout);
        } else if (msg->type == 2) {
            fprintf(stderr, "Server error: %.*s\n", msg->length, msg->value);
        }
    }

    close(sockfd);
    return 0;
}
