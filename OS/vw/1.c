#include <vxWorks.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <eventLib.h>
#include <stdio.h>
#include <string.h>

extern int g_int_data;

#define MSG_Q_LEN 20
#define MSG_SIZE 100

static MSG_Q_ID g_msgQ_id = NULL;

/* Structure for message content */
typedef struct {
    int type;
    int data;
} os_demo_msg_t;

/* Message Receiver Task - DMSG */
void os_demo_msg_recv(void)
{
    os_demo_msg_t msg;

    while (1) {
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER) != ERROR) {
            Print("Task DMSG, Received a message: type %d, data %d\n", msg.type, msg.data, 0, 0, 0, 0, 0);
        }
    }
}

/* Event Receiver Task - DEVT */
void os_demo_event_recv(void)
{
    UINT32 eventReceived;

    while (1) {
        if (eventReceive(0xFFFFFFFF, EVENT_WAIT_OR | EVENT_WAIT_CLR, WAIT_FOREVER, &eventReceived) == OK) {
            for (int i = 0; i < 32; ++i) {
                if (eventReceived & (1 << i)) {
                    Print("Task DEVT,Received event 0x%X\n", (1 << i));
                }
            }
        }
    }
}

/* Command: Create tasks and message queue */
int os_demo_create_msg_event_tasks(int argc, char **argv, struct user *u)
{
    TASK_ID tid_msg, tid_evt;

    if (g_msgQ_id == NULL) {
        g_msgQ_id = msgQCreate(MSG_Q_LEN, sizeof(os_demo_msg_t), MSG_Q_FIFO);
        if (g_msgQ_id == NULL) {
            Print("[Error] Failed to create message queue\n");
            return -1;
        }
    }

    tid_msg = taskSpawn("DMSG", 128, 0, 1024, (FUNCPTR)os_demo_msg_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid_msg == (TASK_ID)-1) {
        Print("[Error] Failed to create DMSG task\n");
        return -1;
    }
    Print("[Info] DMSG Task ID: 0x%X\n", (unsigned int)tid_msg);

    tid_evt = taskSpawn("DEVT", 128, 0, 1024, (FUNCPTR)os_demo_event_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid_evt == (TASK_ID)-1) {
        Print("[Error] Failed to create DEVT task\n");
        return -1;
    }
    Print("[Info] DEVT Task ID: 0x%X\n", (unsigned int)tid_evt);

    return 0;
}

/* Command: Send a message */
int os_demo_send_msg(int argc, char **argv, struct user *u)
{
    if (argc < 3) {
        Print("Usage: os-demo msg <type> <data>\n");
        return -1;
    }

    os_demo_msg_t msg;
    msg.type = atoi(argv[1]);
    msg.data = atoi(argv[2]);

    if (msgQSend(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER, MSG_PRI_NORMAL) != ERROR) {
        Print("Task tty0, sent a message: type %d, data %d\n", msg.type, msg.data);
        return 0;
    } else {
        Print("[Error] Failed to send message\n");
        return -1;
    }

    return 0;
}

/* Command: Send an event */
int os_demo_send_event(int argc, char **argv, struct user *u)
{
    if (argc < 2) {
        Print("Usage: os-demo event <bitmask>\n");
        return -1;
    }

    UINT32 mask = (UINT32)strtoul(argv[1], NULL, 0);
    if (eventSend(taskNameToId("DEVT"), mask) == OK) {
        Print("Task tty0, sent an event 0x%X to the receiving task\n");
        return 0;
    } else {
        Print("[Error] Failed to send event\n");
        return -1;
    }

    return 0;
}


