// server.c
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

int read_file(const char *filename, char **content, uint32_t *length) {
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    *content = malloc(*length + 1);
    if (!*content) {
        fclose(file);
        return -1;
    }

    fread(*content, 1, *length, file);
    (*content)[*length] = '\0';
    fclose(file);
    return 0;
}

void send_tlv(int sock, uint8_t type, const void *value, uint32_t length) {
    uint32_t len_net = htonl(length);
    send(sock, &type, 1, 0);
    send(sock, &len_net, 4, 0);
    send(sock, value, length, 0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    uint8_t type;
    uint32_t len_net, len;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);
    printf("Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
    printf("Client connected.\n");

    while (1) {
        int bytes = recv(client_fd, &type, 1, 0);
        if (bytes <= 0) break;

        recv(client_fd, &len_net, 4, 0);
        len = ntohl(len_net);

        char filename[256] = {0};
        if (len >= sizeof(filename)) len = sizeof(filename) - 1;
        recv(client_fd, filename, len, 0);
        filename[len] = '\0';

        printf("Requested file: %s\n", filename);

        if (strcmp(filename, "exit") == 0) break;

        char *file_content = NULL;
        uint32_t file_length = 0;

        if (read_file(filename, &file_content, &file_length) == 0) {
            send_tlv(client_fd, TYPE_FILEDATA, file_content, file_length);
            free(file_content);
        } else {
            const char *error_msg = "Unable to open file.";
            send_tlv(client_fd, TYPE_ERROR, error_msg, strlen(error_msg));
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
