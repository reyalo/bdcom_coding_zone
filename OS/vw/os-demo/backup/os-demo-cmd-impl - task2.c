//#include <vxWorks.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <eventLib.h>
#include <string.h>
#include <stdio.h>


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
        if (eventReceive(0x1, EVENTS_WAIT_ALL, WAIT_FOREVER, &receivedEvents) == OK)
        {
            Print("Event %d received: 0x%X\n", i++, receivedEvents);
        }
    }
}

/* CLI Trigger Function */
void os_demo_create_msg_event_tasks(void)
{
    TASK_ID tid_msg, tid_evt;

    /* Create message queue if not already created */
    if (!g_msgQ_id)
    {
        g_msgQ_id = msgQCreate(20, 100, MSG_Q_FIFO);
        if (!g_msgQ_id)
        {
            Print("Failed to create message queue\n");
            return;
        }
    }

    /* Create DMSG Task */
    tid_msg = taskSpawn("DMSG", 128, 0, 1024, (FUNCPTR)os_demo_msg_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid_msg == (TASK_ID)-1)
    {
        Print("Failed to spawn DMSG task\n");
        return;
    }

    /* Create DEVT Task */
    tid_evt = taskSpawn("DEVT", 128, 0, 1024, (FUNCPTR)os_demo_event_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid_evt == (TASK_ID)-1)
    {
        Print("Failed to spawn DEVT task\n");
        return;
    }

    Print("DMSG Task ID: 0x%X | DEVT Task ID: 0x%X\n", (int)tid_msg, (int)tid_evt);
}
