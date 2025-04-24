#include <vxWorks.h>
#include <taskLib.h>
#include <sockLib.h>
#include <inetLib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ioLib.h>

#define SERVER_PORT 5000
#define SERVER_STACK_SIZE (1024 * 3)
#define SERVER_PRIORITY 100
#define SERVER_TASK_NAME "SCKT"

#define REMOTE_COMMUNICATION_MSG_TYPE_QRY 1
#define REMOTE_COMMUNICATION_MSG_TYPE_CFM 3

void os_demo_socket_text_process(void) {
    int sock;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char recvBuf[64];
    char sendBuf[64];
    int recvLen;

    sock = so_socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        Print("[Error] so_socket() failed\n", 0, 0, 0, 0, 0, 0);
        return;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (so_bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        Print("[Error] so_bind() failed\n", 0, 0, 0, 0, 0, 0);
        so_close(sock);
        return;
    }

    Print("[UDP Server] Listening on port %d...\n", SERVER_PORT, 0, 0, 0, 0, 0, 0);

    while (1) {
        recvLen = so_recvfrom(sock, recvBuf, sizeof(recvBuf), 0,
                              (struct sockaddr *)&clientAddr, &addrLen);
        if (recvLen < 0) {
            Print("[Error] so_recvfrom() failed\n", 0, 0, 0, 0, 0, 0);
            continue;
        }

        if (recvLen < 2) {
            Print("[Warning] Received message too short\n", 0, 0, 0, 0, 0, 0);
            continue;
        }

        int msgType = recvBuf[0];
        int msgLen  = recvBuf[1];
        if (msgLen > sizeof(recvBuf) - 2) msgLen = sizeof(recvBuf) - 2;

        char msg[61];
        memcpy(msg, &recvBuf[2], msgLen);
        msg[msgLen] = '\0';

        if (msgType == REMOTE_COMMUNICATION_MSG_TYPE_QRY) {
            Print("[Server] Received QRY: %s\n", (int)msg, 0, 0, 0, 0, 0, 0);

            snprintf(sendBuf, sizeof(sendBuf), "Hi %s %d",
                     inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            so_sendto(sock, sendBuf, strlen(sendBuf), 0,
                      (struct sockaddr *)&clientAddr, addrLen);

        } else if (msgType == REMOTE_COMMUNICATION_MSG_TYPE_CFM) {
            Print("[Server] Received CFM: %s\n", (int)msg, 0, 0, 0, 0, 0, 0);

            snprintf(sendBuf, sizeof(sendBuf), "How do you do %s %d",
                     inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            so_sendto(sock, sendBuf, strlen(sendBuf), 0,
                      (struct sockaddr *)&clientAddr, addrLen);
        } else {
            Print("[Warning] Unknown message type: %d\n", msgType, 0, 0, 0, 0, 0, 0);
        }
    }

    so_close(sock);
}

void os_demo_create_socket_task(void) {
    TASK_ID tid = taskSpawn(SERVER_TASK_NAME, SERVER_PRIORITY, 0, SERVER_STACK_SIZE,
                            (FUNCPTR)os_demo_socket_text_process,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid == -1) {
        Print("[Error] Failed to spawn UDP socket server task\n", 0, 0, 0, 0, 0, 0);
    } else {
        Print("[Info] UDP socket server task started (TID: 0x%X)\n", (int)tid, 0, 0, 0, 0, 0);
    }
}
