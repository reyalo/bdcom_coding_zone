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
#include "timer.h"

// ... [previous typedefs and defines remain the same] ...

static MSG_Q_ID g_msgQ_id = NULL;
static ULONG timer_event_id = INVALID_TIMER_ID;
static ULONG timer_msg_id = INVALID_TIMER_ID;

void os_demo_register_cmds(void);

/* Timer callback function for events */
void timer_event_callback(ULONG arg)
{
    UINT32 mask = EVENT_CUSTOM;
    UINT32 tid = taskNameToId(SKEM_TASK_NAME);
    
    if (tid == 0) {
        Print("SKEM task not found\n");
        return;
    }

    if (eventSend(tid, mask) == OK) {
        message_t eventMsg;
        eventMsg.type = MSG_TYPE_EVENT;
        eventMsg.hsocket = 0;
        eventMsg.data = mask;
        eventMsg.u_arg = 0;
        
        if (msgQSend(g_msgQ_id, (char *)&eventMsg, sizeof(eventMsg), NO_WAIT, MSG_PRI_NORMAL) == 0) {
            Print("Timer sent event: type=%d, data=0x%X\n", eventMsg.type, eventMsg.data);
        }
    }
}

/* Timer callback function for messages */
void timer_msg_callback(ULONG arg)
{
    static int msg_count = 0;
    message_t msg;
    
    msg.type = MSG_TYPE_MESSAGE;
    msg.hsocket = 0;
    msg.data = msg_count++;
    msg.u_arg = 0;

    if (msgQSend(g_msgQ_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) == 0) {
        Print("Timer sent message: type=%d, data=%d\n", msg.type, msg.data);
    }
}

/* Initialize and start the timers */
void os_demo_start_timers(void)
{
    TIMER_USER_DATA timer_data;
    ULONG timeout;
    
    // Create and start event timer (callback type)
    timer_data.cb.fun = timer_event_callback;
    timer_data.cb.arg = 0;
    
    if (sys_add_timer(TIMER_CALLBACK_METHOD | TIMER_LOOP, &timer_data, &timer_event_id) != SYS_NOERR) {
        Print("Failed to create event timer\n");
        return;
    }
    
    timeout = TIMER_RESOLUTION_S | 15; // 15 seconds
    if (sys_start_timer(timer_event_id, timeout) != SYS_NOERR) {
        Print("Failed to start event timer\n");
    }
    
    // Create and start message timer (message type)
    timer_data.msg.qid = g_msgQ_id;
    timer_data.msg.msg_buf[0] = MSG_TYPE_MESSAGE;
    timer_data.msg.msg_buf[1] = 0; // Will be incremented in callback
    
    if (sys_add_timer(TIMER_MSG_METHOD | TIMER_LOOP, &timer_data, &timer_msg_id) != SYS_NOERR) {
        Print("Failed to create message timer\n");
        return;
    }
    
    timeout = TIMER_RESOLUTION_S | 15; // 15 seconds
    if (sys_start_timer(timer_msg_id, timeout) != SYS_NOERR) {
        Print("Failed to start message timer\n");
    }
    
    Print("Timers started - sending events and messages every 15 seconds\n");
}

/* Clean up timers */
void os_demo_cleanup_timers(void)
{
    if (timer_event_id != INVALID_TIMER_ID) {
        sys_stop_timer(timer_event_id);
        sys_delete_timer(timer_event_id);
        timer_event_id = INVALID_TIMER_ID;
    }
    
    if (timer_msg_id != INVALID_TIMER_ID) {
        sys_stop_timer(timer_msg_id);
        sys_delete_timer(timer_msg_id);
        timer_msg_id = INVALID_TIMER_ID;
    }
}

/* Message Receiver Function - modified to show timer information */
void os_demo_socket_message_event_process(void)
{   
    message_t msg;
    UINT32 receivedEvents;
    int i, opt = 1;
    
    socket_info_t sockets[MAX_UDP_SOCKETS];
    
    Print("DMNE task running. Initializing sockets and waiting for messages...\n");

    // Initialize UDP sockets (same as before)
    for (i = 0; i < MAX_UDP_SOCKETS; i++) {
        // ... [socket initialization code remains the same] ...
    }
    
    char recvBuf[RECV_BUFFER_SIZE];
    char sendBuf[SEND_BUFFER_SIZE];
    
    // Main message processing loop
    while (1) {
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER) != ERROR) {
            taskDelay(sysClkRateGet() / 10);
            
            if(msg.type == SOCKET_DATARCVD) {
                // ... [socket data handling code remains the same] ...
            }
            else if (msg.type == MSG_TYPE_MESSAGE) {
                Print("Task DMNE received timer message: type=%d, data=%d\n", 
                      msg.type, msg.data);
                
                // Print timer information
                if (timer_msg_id != INVALID_TIMER_ID) {
                    unsigned long remaining = sys_read_timer(timer_msg_id);
                    Print("Message timer info: state=%d, remaining=%lu ms\n",
                          sys_timer_state(timer_msg_id), remaining);
                }
            } 
            else if (msg.type == MSG_TYPE_EVENT) {
                Print("Task DMNE received timer event: type=%d, data=0x%X\n", 
                      msg.type, msg.data);
                
                // Print timer information
                if (timer_event_id != INVALID_TIMER_ID) {
                    unsigned long remaining = sys_read_timer(timer_event_id);
                    Print("Event timer info: state=%d, remaining=%lu ms\n",
                          sys_timer_state(timer_event_id), remaining);
                }

                if (eventReceive(ALL_EVENTS, EVENTS_WAIT_ANY, NO_WAIT, &receivedEvents) == OK) {
                    // ... [event handling code remains the same] ...
                }      
            }
            else {
                Print("Task DMNE received unknown message type: %d\n", msg.type);
            }
        }
    }

    // Cleanup sockets (same as before)
    for(i = 0; i < MAX_UDP_SOCKETS; i++) {
        if(sockets[i].is_active) {
            so_close(sockets[i].sock);
        }
    }
}

/* Init function - modified to start timers */
void os_demo_init(void)
{	
    Print("_______________Hello World_______________(3)\n");
    
    // Create message queue if not already created
    if (!g_msgQ_id) {
        g_msgQ_id = msgQCreate(DMNE_Q_LEN, DMNE_MSG_SIZE, DMNE_Q_OPTIONS);
        if (!g_msgQ_id) {
            Print("Failed to create message queue\n");
            return;
        }
    }
    
    // Create DMNE Task
    UINT32 tid_msg = taskSpawn(SKEM_TASK_NAME, SKEM_TASK_PRI, 0, SKEM_TASK_STACK, 
                              (FUNCPTR)os_demo_socket_message_event_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    if (tid_msg == -1) {
        Print("Failed to spawn SKEM task\n");
        return;
    }
    
    Print("SKEM Task ID: 0x%X \n", (int)tid_msg);
    
    // Start the timers
    os_demo_start_timers();
    
    os_demo_register_cmds();
}