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



//int Print(const char *fmt, ...);

typedef struct {
    int type;
    int hsocket;
    int data;
    int u_arg;
} message_t;

#define DMNE_Q_LEN       	20
#define DMNE_MSG_SIZE    	sizeof(message_t)
#define DMNE_Q_OPTIONS   	MSG_Q_FIFO
#define MAX_DATA_LEN 		100

#define ALL_EVENTS       	0xFFFFFFFF
#define EVENT_MSG_READY  	0x80000000
#define EVENT_CUSTOM     	0x02

#define SKEM_TASK_NAME    	"SKEM"
#define SKEM_TASK_PRI     	128
#define SKEM_TASK_STACK   	1024

#define MSG_TYPE_MESSAGE 1
#define MSG_TYPE_EVENT   2

//#define Print printf
#define MAX_UDP_SOCKETS 5
#define BASE_PORT 5050
#define RECV_BUFFER_SIZE 60

#define REMOTE_COMMUNICATION_MSG_TYPE_QRY 1
#define REMOTE_COMMUNICATION_MSG_TYPE_QRY_ACK 2
#define REMOTE_COMMUNICATION_MSG_TYPE_CFM 3

typedef struct {
    int sock;
    struct soaddr_in addr;
    int port;
    int is_active;
} socket_info_t;

static MSG_Q_ID g_msgQ_id = NULL;

void os_demo_register_cmds(void);

/* Message Receiver Function */
void os_demo_socket_message_event_process(void)
{   
    message_t msg;
    UINT32 receivedEvents;
    int i, opt = 1;
    
    // Move socket_info to be local
    socket_info_t sockets[MAX_UDP_SOCKETS];
    
    Print("DMNE task running. Initializing sockets and waiting for messages...\n");

    // Initialize UDP sockets
    for (i = 0; i < MAX_UDP_SOCKETS; i++) {
        sockets[i].sock = so_socket(AF_INET, SOCK_DGRAM, 0);
        if (sockets[i].sock < 0) {
            Print("[Error] Failed to create UDP socket %d\n", i);
            continue;
        }

        memset(&sockets[i].addr, 0, sizeof(struct soaddr_in));
        sockets[i].addr.sin_family = AF_INET;
        sockets[i].addr.sin_port = htons(BASE_PORT + i);
        sockets[i].addr.sin_addr.s_addr = inet_addr("10.10.10.11");
        sockets[i].port = BASE_PORT + i;

        if (so_bind(sockets[i].sock, (struct soaddr *)&sockets[i].addr, 
                    sizeof(struct soaddr_in)) < 0) {
            Print("[Error] Failed to bind UDP socket %d\n", i);
            so_close(sockets[i].sock);
            sockets[i].sock = -1;
            continue;
        }

        // Set non-blocking
        ioctl(sockets[i].sock, FIONBIO, (char *)&opt);

        // Register socket for events
        socket_register(sockets[i].sock, g_msgQ_id, 0);
        sockets[i].is_active = 1;

        Print("[Info] UDP socket %d listening on port %d\n", i, sockets[i].port);
    }

    // Main message processing loop
    while (1) {
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER) != ERROR) {
            taskDelay(sysClkRateGet() / 10);
            
            if(msg.type == SOCKET_DATARCVD) {
                Print("socket data %d %d\n", msg.type, msg.hsocket);
                
                // Find active socket that received data
                for(i = 0; i < MAX_UDP_SOCKETS; i++) {
                    if(sockets[i].is_active && sockets[i].sock == msg.hsocket) {
                        Print("current active socket %d\n", msg.hsocket);
						//#################################################################
						
						char recvBuf[64];
                        char sendBuf[64];
                        struct soaddr_in clientAddr;
                        int addrLen = sizeof(clientAddr);
                        
                        // Receive data from socket
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

						Print("[Server %d] Received request from %s:%d %s\n", i,
							  inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), filename);
						
						
							  
						//#################################################################
						
                    }
                }
            }
            else if (msg.type == MSG_TYPE_MESSAGE) {
                Print("Task DMNE received message: type=%d, data=%d\n", 
                      msg.type, msg.data);
            } else if (msg.type == MSG_TYPE_EVENT) {
                Print("Task DMNE received event: type=%d, data=0x%X\n", 
                      msg.type, msg.data);

                    if (eventReceive(ALL_EVENTS, EVENTS_WAIT_ANY, NO_WAIT, &receivedEvents) == OK) {
                        UINT32 otherEvents = receivedEvents & ~EVENT_MSG_READY;
                        if (otherEvents) {
                            int j;
                            for (j = 0; j < 32; ++j) {
                                if (otherEvents & (1 << j)) {
                                    Print("Task DMNE, Received event: 0x%X\n", (1 << j));
                                }
                            }
                        }
                    }      
            }
            else {
                Print("Task DMNE received unknown message type: %d\n", msg.type);
            }
        }
    }

    // Cleanup sockets
    for(i = 0; i < MAX_UDP_SOCKETS; i++) {
        if(sockets[i].is_active) {
            so_close(sockets[i].sock);
        }
    }
}

/* CLI msg and event create Function */
void os_demo_create_msg_event_tasks(void)
{
    UINT32 tid_msg;

    /* Create message queue if not already created */
    if (!g_msgQ_id)
    {
        g_msgQ_id = msgQCreate(DMNE_Q_LEN, DMNE_MSG_SIZE, DMNE_Q_OPTIONS);
        if (!g_msgQ_id)
        {
            Print("Failed to create message queue\n");
            return;
        }
    }
	
    /* Create DMNE Task */
    tid_msg = taskSpawn(SKEM_TASK_NAME, SKEM_TASK_PRI, 0, SKEM_TASK_STACK, (FUNCPTR)os_demo_socket_message_event_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (tid_msg == -1)
    {
        Print("Failed to spawn SKEM task\n");
        return;
    }

    Print("SKEM Task ID: 0x%X \n", (int)tid_msg);
}


/* CLI: Send Message */
int os_demo_send_msg(int type, char *data)
{   
    message_t msg;
    msg.type = type;
    msg.hsocket = 0;  // Set appropriate value
    msg.data = atoi(data);  // Convert string to int
    msg.u_arg = 0;  // Set appropriate value

    if (msgQSend(g_msgQ_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) == 0) {
        Print("Task tty0, sent a message: type=%d, data=%d\n", msg.type, msg.data);
        return 0;
    } else {
        Print("Failed to send message to SKEM\n");
        return -1;
    }
}

/* CLI: Send Event */
int os_demo_send_event(UINT32 mask)
{
    UINT32 tid = taskNameToId(SKEM_TASK_NAME);
    message_t eventMsg;

    if (tid == 0) {
        Print("SKEM task not found\n");
        return -1;
    }

    if (eventSend(tid, mask) == OK) {
        eventMsg.type = MSG_TYPE_EVENT;
        eventMsg.hsocket = 0;
        eventMsg.data = mask;  // Store event mask as integer
        eventMsg.u_arg = 0;
        
        if (msgQSend(g_msgQ_id, (char *)&eventMsg, sizeof(eventMsg), NO_WAIT, MSG_PRI_NORMAL) == 0) {
            Print("Task tty0, sent event: type=%d, data=0x%X\n", 
                  eventMsg.type, eventMsg.data);
        }
        return 0;
    }
    return -1;
}

/* Init function - called at boot */
void os_demo_init(void)
{	
	//##########################________________print_on_boot______________###################
	Print("_______________Hello World_______________(3)\n");
	
	os_demo_register_cmds();
}
