#include <vxWorks.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <eventLib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // for atoi, strtoul

/* Global Message Queue ID */
MSG_Q_ID g_msgQ_id;

/* Message Receiver Task */
void os_demo_msg_recv(void)
{
    char msg[100];
    int i = 0;

    Print("DMSG task running. Waiting for messages...\n");
    while (1)
    {
        memset(msg, 0, sizeof(msg));
        if (msgQReceive(g_msgQ_id, msg, sizeof(msg), WAIT_FOREVER) != ERROR)
        {
            Print("Msg %d received: %s\n", i++, (int)msg);
        }
    }
}

/* Event Receiver Task */
void os_demo_event_recv(void)
{
    UINT32 receivedEvents;
    int i = 0;

    Print("DEVT task running. Waiting for events...\n");
    while (1)
    {
        if (eventReceive(0xFFFFFFFF, EVENT_WAIT_OR | EVENT_WAIT_CLR, WAIT_FOREVER, &receivedEvents) == OK)
        {
            for (int j = 0; j < 32; ++j)
            {
                if (receivedEvents & (1 << j))
                {
                    Print("Event %d received: 0x%X\n", i++, (1 << j));
                }
            }
        }
    }
}

/* CLI Trigger Function */
void os_demo_create_msg_event_tasks(void)
{
    TASK_ID tid_msg, tid_evt;

    if (!g_msgQ_id)
    {
        g_msgQ_id = msgQCreate(20, 100, MSG_Q_FIFO);
        if (!g_msgQ_id)
        {
            Print("Failed to create message queue\n");
            return;
        }
    }

    tid_msg = taskSpawn("DMSG", 128, 0, 1024, (FUNCPTR)os_demo_msg_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid_msg == (TASK_ID)-1)
    {
        Print("Failed to spawn DMSG task\n");
        return;
    }

    tid_evt = taskSpawn("DEVT", 128, 0, 1024, (FUNCPTR)os_demo_event_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid_evt == (TASK_ID)-1)
    {
        Print("Failed to spawn DEVT task\n");
        return;
    }

    Print("DMSG Task ID: 0x%X | DEVT Task ID: 0x%X\n", (int)tid_msg, (int)tid_evt);
}

/* CLI: Send Message */
int os_demo_send_msg(int argc, char **argv, struct user *u)
{
    if (argc < 3)
    {
        Print("Usage: os-demo msg <type> <data>\n");
        return -1;
    }

    char msg[100];
    int type = atoi(argv[1]);
    int data = atoi(argv[2]);

    snprintf(msg, sizeof(msg), "type %d, data %d", type, data);

    if (msgQSend(g_msgQ_id, msg, strlen(msg) + 1, WAIT_FOREVER, MSG_PRI_NORMAL) != ERROR)
    {
        Print("Task tty0, sent a message: %s\n", (int)msg);
        return 0;
    }
    else
    {
        Print("Failed to send message\n");
        return -1;
    }
}

/* CLI: Send Event */
int os_demo_send_event(int argc, char **argv, struct user *u)
{
    if (argc < 2)
    {
        Print("Usage: os-demo event <bitmask>\n");
        return -1;
    }

    UINT32 mask = (UINT32)strtoul(argv[1], NULL, 0);
    TASK_ID tid = taskNameToId("DEVT");

    if (tid == 0)
    {
        Print("DEVT task not found\n");
        return -1;
    }

    if (eventSend(tid, mask) == OK)
    {
        Print("Task tty0, sent an event 0x%X to the receiving task\n", mask);
        return 0;
    }
    else
    {
        Print("Failed to send event\n");
        return -1;
    }
}
