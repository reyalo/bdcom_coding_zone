// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define TYPE_FILENAME 0x01
#define TYPE_FILEDATA 0x02
#define TYPE_ERROR    0x03

void send_tlv(int sock, uint8_t type, const void *value, uint32_t length) {
    uint32_t len_net = htonl(length);
    send(sock, &type, 1, 0);
    send(sock, &len_net, 4, 0);
    send(sock, value, length, 0);
}

int recv_tlv(int sock, uint8_t *type, char **value, uint32_t *length) {
    uint32_t len_net;

    if (recv(sock, type, 1, 0) <= 0) return -1;
    if (recv(sock, &len_net, 4, 0) <= 0) return -1;

    *length = ntohl(len_net);
    *value = malloc(*length + 1);
    if (!*value) return -1;

    if (recv(sock, *value, *length, 0) <= 0) {
        free(*value);
        return -1;
    }

    (*value)[*length] = '\0';
    return 0;
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char filename[256];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while (1) {
        printf("Enter filename (or 'exit' to quit): ");
        scanf("%s", filename);

        send_tlv(sock, TYPE_FILENAME, filename, strlen(filename));

        if (strcmp(filename, "exit") == 0) break;

        uint8_t type;
        char *data = NULL;
        uint32_t length;

        if (recv_tlv(sock, &type, &data, &length) == 0) {
            if (type == TYPE_FILEDATA) {
                printf("File content:\n%s\n", data);
            } else if (type == TYPE_ERROR) {
                printf("Server Error: %s\n", data);
            }
            free(data);
        } else {
            printf("Error receiving response.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
