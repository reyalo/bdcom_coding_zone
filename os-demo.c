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
#include <libsys/timer.h>




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
#define RECV_BUFFER_SIZE 64
#define SEND_BUFFER_SIZE 64

#define REMOTE_COMMUNICATION_MSG_TYPE_QRY 1
#define REMOTE_COMMUNICATION_MSG_TYPE_QRY_ACK 2
#define REMOTE_COMMUNICATION_MSG_TYPE_CFM 3


static MSG_Q_ID g_msgQ_id = NULL;
static ULONG timer_event_id = INVALID_TIMER_ID;
static ULONG timer_msg_id = INVALID_TIMER_ID;

void os_demo_register_cmds(void);

/* Message Receiver Function */
void os_demo_socket_message_event_process(void)
{   
    message_t msg;
    UINT32 receivedEvents;

    // Main message processing loop
    while (1) {
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER) != ERROR) {
            taskDelay(sysClkRateGet() / 10);
            
            if (msg.type == MSG_TYPE_MESSAGE) {
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

/* Timer callback function for sending events */
void timer_event_callback(ULONG arg) {
    UINT32 mask = EVENT_CUSTOM;
    UINT32 tid = taskNameToId(SKEM_TASK_NAME);
    message_t eventMsg;

    // Send event
    if (eventSend(tid, mask) == OK) {
        // Prepare and send corresponding message
        eventMsg.type = MSG_TYPE_EVENT;
        eventMsg.hsocket = 0;
        eventMsg.data = mask;
        eventMsg.u_arg = 0;
        
        if (msgQSend(g_msgQ_id, (char *)&eventMsg, sizeof(eventMsg), NO_WAIT, MSG_PRI_NORMAL) == 0) {
            Print("[Timer] Event and message sent to SKEM task with mask: 0x%X\n", mask);
        }
    }
}

/* Timer callback function for sending messages */
void timer_msg_callback(ULONG arg) {
    char data[10];
    snprintf(data, sizeof(data), "%d", arg); // Convert arg to string
    os_demo_send_msg(MSG_TYPE_MESSAGE, data); // Send the message to the SKEM task
    Print("[Timer] Message sent to SKEM task with data: %s\n", data);
}

/* Function to create and start timers */
void os_demo_create_timers(void) {
    TIMER_USER_DATA timer_event_data;
    TIMER_USER_DATA timer_msg_data;

    // Configure timer_event as a callback type
    timer_event_data.cb.fun = timer_event_callback;
    timer_event_data.cb.arg = 0; // No additional argument

    // Configure timer_msg data to include the message content
    timer_msg_data.msg.qid = g_msgQ_id;
    message_t msg = {
        .type = MSG_TYPE_MESSAGE,
        .hsocket = 0,
        .data = 42,
        .u_arg = 0
    };
    // Copy message structure into timer buffer
    memcpy(timer_msg_data.msg.msg_buf, &msg, sizeof(message_t));

    // Create and start event timer
    if (sys_add_timer(TIMER_CALLBACK_METHOD | TIMER_LOOP, &timer_event_data, &timer_event_id) == SYS_NOERR) {
        sys_start_timer(timer_event_id, 15 | TIMER_RESOLUTION_S);
        Print("[Timer] timer_event created and started successfully.\n");
    } else {
        Print("[Error] Failed to create timer_event.\n");
    }

    // Create and start message timer
    if (sys_add_timer(TIMER_MSG_METHOD | TIMER_LOOP, &timer_msg_data, &timer_msg_id) == SYS_NOERR) {
        sys_start_timer(timer_msg_id, 15 | TIMER_RESOLUTION_S);
        Print("[Timer] timer_msg created and started successfully.\n");
    } else {
        Print("[Error] Failed to create timer_msg.\n");
    }
}

/* Init function - called at boot */
void os_demo_init(void)
{	
	//##########################________________print_on_boot______________###################
	Print("_______________Hello World_______________(3)\n");
	
	os_demo_register_cmds();
    os_demo_create_msg_event_tasks();
    os_demo_create_timers(); // Initialize and start timers
}
