#include <ip/types.h>
#include <ip/in.h>
#include <ip/inet.h>
#include <ip/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <global.h>
#include <ip/target.h>

#include <libcmd/cmdparse.h>
#include <ip/trace.h>
#include <ip/ip_compat.h>
#include <ip/systm.h>
#include <libsys/memory.h>
#if defined(OS_VXWORKS) || defined(OS_LINUX)
#include <libsys/vos/vos_msgq.h>
#include <libsys/vos/vos_task.h>
#endif

#include <ip/ioctl.h>
#include <ip/msg.h>
#include <ip/errno.h>

/*function prototype*/
void os_demo_register_cmds(void);

//#define Print printf
#define SERVER_PORT 5050
#define SERVER_STACK_SIZE (1024 * 3)
#define SERVER_PRIORITY 100
#define SERVER_TASK_NAME "SCKT"

#define REMOTE_COMMUNICATION_MSG_TYPE_QRY 1
#define REMOTE_COMMUNICATION_MSG_TYPE_CFM 3

void os_demo_socket_text_process(void) {
    int sock;
    struct soaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char recvBuf[64];
    char sendBuf[64];
    int recvLen;

    sock = so_socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        Print("[Error] so_socket() failed\n");
        return;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("10.10.10.11");

    if (so_bind(sock, (struct soaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        Print("[Error] so_bind() failed\n");
        so_close(sock);
        return;
    }

    Print("[UDP Server] Listening on port %d...\n", SERVER_PORT);

    while (1) {
        recvLen = so_recvfrom(sock, recvBuf, sizeof(recvBuf), 0,
                              (struct soaddr *)&clientAddr, &addrLen);
        if (recvLen < 0) {
            Print("[Error] so_recvfrom() failed\n");
            continue;
        }

        if (recvLen < 2) {
            Print("[Warning] Received message too short\n");
            continue;
        }

        int msgType = recvBuf[0];
        int msgLen  = recvBuf[1];
        if (msgLen > sizeof(recvBuf) - 2) msgLen = sizeof(recvBuf) - 2;

        char msg[61];
        memcpy(msg, &recvBuf[2], msgLen);
        msg[msgLen] = '\0';

        if (msgType == REMOTE_COMMUNICATION_MSG_TYPE_QRY) {
            Print("[Server] Received QRY: %s\n", (int)msg);

            snprintf(sendBuf, sizeof(sendBuf), "Hi %s %d",
                     inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            so_sendto(sock, sendBuf, strlen(sendBuf), 0,
                      (struct soaddr *)&clientAddr, addrLen);

        } else if (msgType == REMOTE_COMMUNICATION_MSG_TYPE_CFM) {
            Print("[Server] Received CFM: %s\n", (int)msg);

            snprintf(sendBuf, sizeof(sendBuf), "How do you do %s %d",
                     inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            so_sendto(sock, sendBuf, strlen(sendBuf), 0,
                      (struct soaddr *)&clientAddr, addrLen);
        } else {
            Print("[Warning] Unknown message type: %d\n", msgType);
        }
    }

    so_close(sock);
}

void os_demo_create_socket_task(void) {
    TASK_ID tid = taskSpawn(SERVER_TASK_NAME, SERVER_PRIORITY, 0, SERVER_STACK_SIZE,
                            (FUNCPTR)os_demo_socket_text_process,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid == -1) {
        Print("[Error] Failed to spawn UDP socket server task\n");
    } else {
        Print("[Info] UDP socket server task started (TID: 0x%X)\n", (int)tid);
    }
}

/* Init function - called at boot */
void os_demo_init(void)
{	
	//##########################________________print_on_boot______________###################
	Print("_______________Hello World_______________(1)\n");
	
	os_demo_register_cmds();
}
