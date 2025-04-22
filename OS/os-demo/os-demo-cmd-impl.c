#include <vxWorks.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <eventLib.h>
#include <string.h>
#include <stdio.h>
#include <sysLib.h>


int Print(const char *fmt, ...);

typedef struct {
    int type;
    char data[100];
} message_t;

#define DMSG_Q_LEN       20
#define DMSG_MSG_SIZE    sizeof(message_t)
#define DMSG_Q_OPTIONS   MSG_Q_FIFO
#define MAX_DATA_LEN 100
static MSG_Q_ID g_msgQ_id = NULL;



/* Global Message Queue ID */
//MSG_Q_ID g_msgQ_id;

/* Message Receiver Task */
void os_demo_msg_recv(void)
{
	
	message_t msg;
	Print("DMSG task running. Waiting for messages...\n");
    while (1) {
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER) != ERROR) {
			//sleep(1);
			taskDelay(sysClkRateGet() / 10); // 10ms if clock = 1000Hz
            Print("Task DMSG received message: type %d, data %s\n", msg.type, msg.data);
        }
		
    }
	
}

// Event Receiver Task 


void os_demo_event_recv(void)
{
    UINT32 receivedEvents;
    //int i = 0;

    Print("DEVT task running. Waiting for events...\n");
    while (1)
    {
        if (eventReceive(0xFFFFFFFF, EVENTS_WAIT_ANY, WAIT_FOREVER, &receivedEvents) == OK)
        {
			int j=0;
            for (j = 0; j < 32; ++j)
            {
                if (receivedEvents & (1 << j))
                {
                    Print("Task DEVT, Received event: 0x%X\n", (1 << j));
                }
            }
        }
    }
}

/* CLI Trigger Function */
void os_demo_create_msg_event_tasks(void)
{
    UINT32 tid_msg, tid_evt;

    /* Create message queue if not already created */
    if (!g_msgQ_id)
    {
        g_msgQ_id = msgQCreate(DMSG_Q_LEN, DMSG_MSG_SIZE, DMSG_Q_OPTIONS);
        if (!g_msgQ_id)
        {
            Print("Failed to create message queue\n");
            return;
        }
    }

    /* Create DMSG Task */
    tid_msg = taskSpawn("DMSG", 128, 0, 1024, (FUNCPTR)os_demo_msg_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid_msg == -1)
    {
        Print("Failed to spawn DMSG task\n");
        return;
    }

    /* Create DEVT Task */
    tid_evt = taskSpawn("DEVT", 128, 0, 1024, (FUNCPTR)os_demo_event_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (tid_evt == -1)
    {
        Print("Failed to spawn DEVT task\n");
        return;
    }

    Print("DMSG Task ID: 0x%X | DEVT Task ID: 0x%X\n", (int)tid_msg, (int)tid_evt);
}


int os_demo_send_msg(int argc, char **argv)
{	
	
    if (argc < 3) {
        printf("Usage: os-demo msg <type> <data>\n");
        return -1;
    }

    //type = atoi(argv[1]);
    //data = argv[2];
	
	message_t msg;
	msg.type = atoi(argv[1]);
	strncpy(msg.data, argv[2], MAX_DATA_LEN - 1);
    msg.data[MAX_DATA_LEN - 1] = '\0';
	


    if (msgQSend(g_msgQ_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) == 0) {
        printf("Task tty0, sent a message: type %d, data %s\n", msg.type, msg.data);
        return 0;
    } else {
        printf("Failed to send message to DMSG\n");
        return -1;
    }
}


/* CLI: Send Event */
int os_demo_send_event(int argc, char **argv)
{
    if (argc < 2)
    {
        Print("Usage: os-demo event <bitmask>\n");
        return -1;
    }

    UINT32 mask = (UINT32)strtoul(argv[1], NULL, 0);
    UINT32 tid = taskNameToId("DEVT");

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

