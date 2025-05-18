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
#define EVENT_CUSTOM     	0x07
#define MSG_CUSTOM     	    0x20

#define TASK_NAME    	"DMNE"
#define TASK_PRI     	128
#define TASK_STACK   	1024

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
    while (1)
    {
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER) != ERROR)
        {   
            if (msg.type == MSG_TYPE_MESSAGE)
            {
                Print("Task DMNE received message: type=%d, data=%d\n", 
                msg.type, msg.data);
            }
            else if (msg.type == MSG_TYPE_EVENT)
            {
                Print("Task DMNE received event: 0x%X\n", 
                msg.type, msg.data);

                if (eventReceive(ALL_EVENTS, EVENTS_WAIT_ANY, NO_WAIT, &receivedEvents) == OK) {
                    UINT32 otherEvents = receivedEvents & ~EVENT_MSG_READY;
                    if (otherEvents)
                    {
                        int j;
                        for (j = 0; j < 32; ++j)
                        {
                            if (otherEvents & (1 << j))
                            {
                                Print("Task DMNE, Received event: 0x%X\n", (1 << j));
                            }
                        }
                    }
                }      
            }
            else
            {
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
    tid_msg = taskSpawn(TASK_NAME, TASK_PRI, 0, TASK_STACK, (FUNCPTR)os_demo_socket_message_event_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

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
    return 0;
}

/* CLI: Send Event */
int os_demo_send_event(UINT32 mask)
{
    return 0;
}

/* Timer callback function for sending events */
void timer_event_callback(ULONG arg)
{
    UINT32 tid = taskNameToId(TASK_NAME);
    message_t event_msg;

    // Send event
    if (eventSend(tid, arg) == OK)
    {
        // Prepare and send corresponding message
        event_msg.type = MSG_TYPE_EVENT;
        event_msg.hsocket = 0;
        event_msg.data = arg;
        event_msg.u_arg = 0;
        
        if (msgQSend(g_msgQ_id, (char *)&event_msg, sizeof(event_msg), NO_WAIT, MSG_PRI_NORMAL) == 0)
        {
            Print("[Timer] Event and message sent to DMNE task with mask: 0x%X\n", arg);
        }
    }
}

/* Function to create and start timers */
void os_demo_create_timers(void)
{
    TIMER_USER_DATA timer_event_data;
    TIMER_USER_DATA timer_msg_data;

    // Configure timer_event as a callback type
    timer_event_data.cb.fun = timer_event_callback;
    timer_event_data.cb.arg = EVENT_CUSTOM;

    // Configure timer_msg data to include the message content
    timer_msg_data.msg.qid = g_msgQ_id;
    message_t msg =
    {
        .type = MSG_TYPE_MESSAGE,
        .hsocket = 0,
        .data = 42,
        .u_arg = 0
    };
    // Copy message structure into timer buffer
    memcpy(timer_msg_data.msg.msg_buf, &msg, sizeof(message_t));

    // Create and start event timer
    if (sys_add_timer(TIMER_CALLBACK_METHOD | TIMER_LOOP, &timer_event_data, &timer_event_id) == SYS_NOERR)
    {
        sys_start_timer(timer_event_id, 15 | TIMER_RESOLUTION_S);
        Print("[Timer] timer_event created and started successfully.\n");
    }
    else
    {
        Print("[Error] Failed to create timer_event.\n");
    }

    sleep(5);           // for debug

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
	Print("_______________Hello World_______________(4)\n");
	
	os_demo_register_cmds();
    os_demo_create_msg_event_tasks();
    os_demo_create_timers(); // Initialize and start timers
}
