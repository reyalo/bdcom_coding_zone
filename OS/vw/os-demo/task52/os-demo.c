// #include <vxWorks.h>
// #include <taskLib.h>
// #include <logLib.h>
// #include <stdio.h>
// #include <sysLib.h>
// #include <unistd.h> 
// #include <string.h>
// #include <msgQLib.h>
// #include <eventLib.h>
// #include <signal.h>
// #include <semLib.h>


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
    char data[100];
} message_t;

#define DMNE_Q_LEN       	20
#define DMNE_MSG_SIZE    	sizeof(message_t)
#define DMNE_Q_OPTIONS   	MSG_Q_FIFO
#define MAX_DATA_LEN 		100

#define ALL_EVENTS       	0xFFFFFFFF
#define EVENT_MSG_READY  	0x80000000
#define EVENT_CUSTOM     	0x02

#define DMNE_TASK_NAME    	"DMNE"
#define DMNE_TASK_PRI     	128
#define DMNE_TASK_STACK   	1024

#define MSG_TYPE_MESSAGE 1
#define MSG_TYPE_EVENT   2

//#define Print printf
#define MAX_SOCKETS 5
#define SERVER_PORT 5050

#define SERVER_STACK_SIZE (1024 * 3)
//#define SERVER_PRIORITY 100
//#define SERVER_TASK_NAME "SCKT"

#define RECV_BUFFER_SIZE 64
#define SEND_BUFFER_SIZE 64

static MSG_Q_ID g_msgQ_id = NULL;

void os_demo_register_cmds(void);


/* Message Receiver Function */
void os_demo_msg_recv(void)
{   
    // socket event handling

    int sock;
    struct soaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char recvBuf[RECV_BUFFER_SIZE];
    char sendBuf[SEND_BUFFER_SIZE];  // Can expand as needed
    int recvLen;

    sock = so_socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        Print("[Error] so_socket() failed\n");
        return;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("10.10.10.11");  // Switch's IP

    if (so_bind(sock, (struct soaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        Print("[Error] so_bind() failed\n");
        so_close(sock);
        return;
    }

    // Set non-blocking
    int opt = 1;
    ioctl(sock, FIONBIO, (char *)&opt);

    // Register socket into event/message system
    socket_register(sock, g_msgQ_id, 0);

    fd_set readfds;
    int maxfd;
    int ret;


    //###############################################################################


    message_t msg;
    UINT32 receivedEvents;
    
    Print("DMNE task running. Waiting for messages and events...\n");

    while (1) {
        // Block on message queue with WAIT_FOREVER
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER) != ERROR) {
            taskDelay(sysClkRateGet() / 10);
            
            // Handle different message types
            if (msg.type == MSG_TYPE_MESSAGE) {
                Print("Task DMNE received message: data %s\n", msg.data);
            } else if (msg.type == MSG_TYPE_EVENT) {
                Print("Task DMNE received event notification: %s\n", msg.data);
            }
            else{
                Print("Task DMNE received unknown message type: %d\n", msg.type);
            }
        }
        
        // Check for any pending events (non-blocking)
        if (eventReceive(ALL_EVENTS, EVENTS_WAIT_ANY, NO_WAIT, &receivedEvents) == OK) {
            // Handle other user-defined events
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
    tid_msg = taskSpawn(DMNE_TASK_NAME, DMNE_TASK_PRI, 0, DMNE_TASK_STACK, (FUNCPTR)os_demo_msg_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (tid_msg == -1)
    {
        Print("Failed to spawn DMNE task\n");
        return;
    }

    Print("DMNE Task ID: 0x%X \n", (int)tid_msg);
}


/* CLI: Send Message */
int os_demo_send_msg(int type, char *data)
{   
    message_t msg;

    msg.type = type;
    strncpy(msg.data, data, MAX_DATA_LEN - 1);

    if (msgQSend(g_msgQ_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) == 0) {
        // Remove eventSend since it's not needed
        Print("Task tty0, sent a message: type %d, data %s\n", msg.type, msg.data);
        return 0;
    } else {
        Print("Failed to send message to DMNE\n");
        return -1;
    }
}

/* CLI: Send Event */
int os_demo_send_event(UINT32 mask)
{
    UINT32 tid = taskNameToId(DMNE_TASK_NAME);
    message_t eventMsg;

    if (tid == 0) {
        Print("DMNE task not found\n");
        return -1;
    }

    // Send the event
    if (eventSend(tid, mask) == OK) {
        
        // Also send a message indicating event occurrence
        eventMsg.type = MSG_TYPE_EVENT;
        snprintf(eventMsg.data, MAX_DATA_LEN - 1, "E:0x%X", mask);
        
        if (msgQSend(g_msgQ_id, (char *)&eventMsg, sizeof(eventMsg), NO_WAIT, MSG_PRI_NORMAL) == 0) {
            Print("Task tty0, sent event notification message\n");
        }
        
        Print("Task tty0, sent an event 0x%X to the receiving task\n", mask);
        return 0;
    } else {
        Print("Failed to send event\n");
        return -1;
    }
}

/* Init function - called at boot */
void os_demo_init(void)
{	
	//##########################________________print_on_boot______________###################
	Print("_______________Hello World_______________(3)\n");
	
	os_demo_register_cmds();
}
