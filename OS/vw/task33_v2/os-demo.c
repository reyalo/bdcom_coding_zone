// File: os-demo(1).c
#include <ip/types.h>
#include <ip/in.h>
#include <ip/inet.h>
#include <ip/socket.h>
#include <libfile/file_sys.h>


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
#define MAX_SOCKETS 5
#define BASE_PORT 5050

#define SERVER_STACK_SIZE (1024 * 3)
#define SERVER_PRIORITY 100
#define SERVER_TASK_NAME "SCKT"

#define RECV_BUFFER_SIZE 64
#define SEND_BUFFER_SIZE 64

#define REMOTE_COMMUNICATION_MSG_TYPE_QRY 1
#define REMOTE_COMMUNICATION_MSG_TYPE_QRY_ACK 2
#define REMOTE_COMMUNICATION_MSG_TYPE_CFM 3


typedef struct {
    int sock;
    UINT16 port;
    struct soaddr_in addr;
} socket_info_t;


void os_demo_socket_text_process(void) {
    socket_info_t sockets[MAX_SOCKETS];
    fd_set readfds;
    int max_fd = 0;
    struct timeval tv;

    // Initialize all sockets
    int i;
    for (i = 0; i < MAX_SOCKETS; i++) {
        sockets[i].sock = so_socket(AF_INET, SOCK_DGRAM, 0);
        if (sockets[i].sock < 0) {
            Print("[Error] Failed to create socket %d\n", i);
            continue;
        }

        memset(&sockets[i].addr, 0, sizeof(struct soaddr_in));
        sockets[i].addr.sin_family = AF_INET;
        sockets[i].addr.sin_port = htons(BASE_PORT + i);
        sockets[i].addr.sin_addr.s_addr = inet_addr("10.10.10.11");     // return an integer of big-endian format
        sockets[i].port = BASE_PORT + i;

        if (so_bind(sockets[i].sock, (struct soaddr *)&sockets[i].addr, 
                    sizeof(struct soaddr_in)) < 0) {
            Print("[Error] Failed to bind socket %d\n", i);
            so_close(sockets[i].sock);
            sockets[i].sock = -1;
            continue;
        }

        Print("[UDP Server] Socket %d listening on port %d\n", i, sockets[i].port);
        
        if (sockets[i].sock > max_fd) {
            max_fd = sockets[i].sock;
        }
    }

    char recvBuf[RECV_BUFFER_SIZE];
    char sendBuf[SEND_BUFFER_SIZE];

    while (1) {
        FD_ZERO(&readfds);
   
        for (i = 0; i < MAX_SOCKETS; i++) {
            if (sockets[i].sock >= 0) {
                FD_SET(sockets[i].sock, &readfds);
            }
        }

        // Set timeout for select
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int activity = so_select(max_fd + 1, &readfds, NULL, NULL, NULL);
        
        if (activity < 0) {
            Print("[Error] select error\n");
            continue;
        }

        if (activity == 0) {
            Print("[Info] No data within timeout period\n");
            continue;
        }

        // Check all sockets for activity
    
        for (i = 0; i < MAX_SOCKETS; i++) {
            if (sockets[i].sock < 0) continue;

            if (FD_ISSET(sockets[i].sock, &readfds)) {
                struct soaddr_in clientAddr;
                int addrLen = sizeof(clientAddr);
                int recvLen = so_recvfrom(sockets[i].sock, recvBuf, sizeof(recvBuf), 0,
                                        (struct soaddr *)&clientAddr, &addrLen);

                if (recvLen < 2) {
                    Print("[Warning] Message too short on socket %d\n", i);
                    continue;
                }

                int msgType = recvBuf[0];
                int msgLen = recvBuf[1];
                if (msgLen > sizeof(recvBuf) - 2) {
                    msgLen = sizeof(recvBuf) - 2;
                }

                char filename[RECV_BUFFER_SIZE];
                memcpy(filename, &recvBuf[2], msgLen);
                filename[msgLen] = '\0';

                Print("[Server %d] Received request from %s:%d for file: %s\n", i,
                      inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), filename);

                // Handle file request
                if (msgType == REMOTE_COMMUNICATION_MSG_TYPE_QRY) {
                    FCB_POINT *fp = file_open(filename, "r", NULL);
                    if (!fp) {
                        // Send error response
                        char errMsg[60];
                        int errLen = snprintf(errMsg, sizeof(errMsg), 
                                            "ERROR: Cannot open '%s'", filename);
                        sendBuf[0] = REMOTE_COMMUNICATION_MSG_TYPE_QRY;
                        sendBuf[1] = errLen;
                        memcpy(&sendBuf[2], errMsg, errLen);
                        so_sendto(sockets[i].sock, sendBuf, errLen + 2, 0,
                                (struct soaddr *)&clientAddr, addrLen);
                        continue;
                    }

                    // Send file contents
                    while (1) {
                        memset(sendBuf, 0, sizeof(sendBuf));
                        int readLen = file_read(fp, sendBuf + 2, sizeof(sendBuf) - 3);
                        
                        if (readLen <= 0) break;

                        sendBuf[0] = REMOTE_COMMUNICATION_MSG_TYPE_QRY_ACK;
                        sendBuf[1] = readLen;

                        if (so_sendto(sockets[i].sock, sendBuf, readLen + 2, 0,
                                    (struct soaddr *)&clientAddr, addrLen) < 0) {
                            Print("[Server %d] Send error\n", i);
                            break;
                        }
                    }

                    file_close(fp);

                    // Send EOF marker
                    sendBuf[0] = REMOTE_COMMUNICATION_MSG_TYPE_QRY_ACK;
                    sendBuf[1] = 0;
                    so_sendto(sockets[i].sock, sendBuf, 2, 0,
                             (struct soaddr *)&clientAddr, addrLen);
                }
            }
        }
    }

    // Cleanup

    for (i = 0; i < MAX_SOCKETS; i++) {
        if (sockets[i].sock >= 0) {
            so_close(sockets[i].sock);
        }
    }
}



void os_demo_create_socket_task(void) {
    UINT32 tid = taskSpawn(SERVER_TASK_NAME, SERVER_PRIORITY, 0, SERVER_STACK_SIZE,
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
