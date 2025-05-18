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
#define MSG_CUSTOM     	    0x40
#define TIME_PERIOD     	15

#define TASK_NAME    	    "DMNE"
#define TASK_PRI     	    128
#define TASK_STACK   	    1024

#define MSG_TYPE_MESSAGE 1
#define MSG_TYPE_EVENT   2

static MSG_Q_ID g_msgQ_id = NULL;
static ULONG timer_event_id = INVALID_TIMER_ID;
static ULONG timer_msg_id = INVALID_TIMER_ID;

void os_demo_create_msg_event_tasks(void);
void os_demo_create_timers(void);
void os_demo_timer_message_event_process(void);
void timer_event_callback(ULONG arg);
void os_demo_cleanup_timers(void);

/* Init function - called at boot */
void os_demo_init(void)
{	
	//________________print_on_boot______________
	Print("_______________REYAL_______________(4)\n");
	
	//os_demo_register_cmds();
    os_demo_create_msg_event_tasks();
}

/* Message Receiver Function */
void os_demo_timer_message_event_process(void)
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
                Print("Task DMNE received event: 0x%X\n", msg.data);

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
    tid_msg = taskSpawn(TASK_NAME, TASK_PRI, 0, TASK_STACK, (FUNCPTR)os_demo_timer_message_event_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (tid_msg == -1)
    {
        Print("Failed to spawn DMNE task\n");
        return;
    }

    Print("DMNE Task ID: 0x%X \n", (int)tid_msg);

    os_demo_create_timers(); // Initialize and start timers

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

/* Enhanced timer callback function */
void timer_event_callback(ULONG arg)
{
    UINT32 tid;
    message_t event_msg;
    int result;

    tid = taskNameToId(TASK_NAME);
    if (tid == ERROR)
    {
        Print("[Error] Failed to find task %s\n", TASK_NAME);
        return;
    }

    // Send event
    result = eventSend(tid, arg);
    if (result != OK)
    {
        Print("[Error] Failed to send event to task: %d\n", result);
        return;
    }

    // Prepare message
    memset(&event_msg, 0, sizeof(message_t));
    event_msg.type = MSG_TYPE_EVENT;
    event_msg.hsocket = 0;
    event_msg.data = arg;
    event_msg.u_arg = 0;
    
    // Send message
    result = msgQSend(g_msgQ_id, (char *)&event_msg, sizeof(event_msg), NO_WAIT, MSG_PRI_NORMAL);
    if (result != 0)
    {
        Print("[Error] Failed to send message: %d\n", result);
        return;
    }

    Print("[Timer] Event and message sent to %s task with mask: 0x%X\n", TASK_NAME, arg);

}

/* Timer cleanup function */
void os_demo_cleanup_timers(void)
{
    if (timer_event_id != INVALID_TIMER_ID)
    {
        sys_stop_timer(timer_event_id);
        sys_delete_timer(timer_event_id);
        timer_event_id = INVALID_TIMER_ID;
    }
    if (timer_msg_id != INVALID_TIMER_ID)
    {
        sys_stop_timer(timer_msg_id);
        sys_delete_timer(timer_msg_id);
        timer_msg_id = INVALID_TIMER_ID;
    }
}

/* Enhanced timer creation function */
void os_demo_create_timers(void)
{
    TIMER_USER_DATA timer_event_data;
    TIMER_USER_DATA timer_msg_data;
    int result = 0;

    // Cleanup any existing timers
    os_demo_cleanup_timers();

    // Validate message queue
    if (!g_msgQ_id)
    {
        Print("[Error] Message queue not initialized\n");
        return;
    }

    // Configure timer_event
    memset(&timer_event_data, 0, sizeof(TIMER_USER_DATA));
    timer_event_data.cb.fun = timer_event_callback;
    timer_event_data.cb.arg = EVENT_CUSTOM;

    // Configure timer_msg
    memset(&timer_msg_data, 0, sizeof(TIMER_USER_DATA));
    timer_msg_data.msg.qid = g_msgQ_id;
    message_t msg =
    {
        .type = MSG_TYPE_MESSAGE,
        .hsocket = 0,
        .data = MSG_CUSTOM,
        .u_arg = 0
    };

    if (sizeof(msg) > sizeof(timer_msg_data.msg.msg_buf))
    {
        Print("[Error] Message size exceeds buffer size\n");
        return;
    }
    memcpy(timer_msg_data.msg.msg_buf, &msg, sizeof(msg));

    // Create and start event timer
    result = sys_add_timer(TIMER_CALLBACK_METHOD | TIMER_LOOP, &timer_event_data, &timer_event_id);
    if (result == SYS_NOERR)
    {
        result = sys_start_timer(timer_event_id, TIME_PERIOD | TIMER_RESOLUTION_S);
        if (result == SYS_NOERR) 
        {
            Print("[Timer] timer_event created and started successfully.\n");
        } else 
        {
            Print("[Error] Failed to start timer_event: %d\n", result);
            sys_delete_timer(timer_event_id);
            timer_event_id = INVALID_TIMER_ID;
            return;
        }
    } else 
    {
        Print("[Error] Failed to create timer_event: %d\n", result);
        return;
    }

    // Create and start message timer
    result = sys_add_timer(TIMER_MSG_METHOD | TIMER_LOOP, &timer_msg_data, &timer_msg_id);
    if (result == SYS_NOERR)
    {
        result = sys_start_timer(timer_msg_id, TIME_PERIOD | TIMER_RESOLUTION_S);
        if (result == SYS_NOERR)
        {
            Print("[Timer] timer_msg created and started successfully.\n");
        } else 
        {
            Print("[Error] Failed to start timer_msg: %d\n", result);
            sys_delete_timer(timer_msg_id);
            timer_msg_id = INVALID_TIMER_ID;
            os_demo_cleanup_timers(); // Cleanup event timer as well
            return;
        }
    } else 
    {
        Print("[Error] Failed to create timer_msg: %d\n", result);
        os_demo_cleanup_timers(); // Cleanup event timer
        return;
    }
}

