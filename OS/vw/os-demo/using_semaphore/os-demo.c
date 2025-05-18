#include <vxWorks.h>
#include <taskLib.h>
#include <logLib.h>
#include <stdio.h>
#include <sysLib.h>
#include <unistd.h> 
#include <string.h>
#include <msgQLib.h>
#include <eventLib.h>
#include <signal.h>
#include <semLib.h>

int Print(const char *fmt, ...);

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

static MSG_Q_ID g_msgQ_id = NULL;
SEM_ID g_dmne_sem;  // Global semaphore

/*function prototype*/
void os_demo_register_cmds(void);

/* Message Receiver Function */
void os_demo_msg_recv(void)
{
	
	UINT32 receivedEvents;
	message_t msg;
	
	Print("DMNE task running. Waiting for messages and event...\n");

    while (1) {
		
		semTake(g_dmne_sem, WAIT_FOREVER);  // Only blocking point
		
		if (eventReceive(ALL_EVENTS, EVENTS_WAIT_ANY, NO_WAIT, &receivedEvents) == OK)
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
				
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), NO_WAIT) != ERROR) {
			//sleep(1);
			taskDelay(sysClkRateGet() / 10); // 10ms if clock = 1000Hz
            Print("Task DMSG received message: type %d, data %s\n", msg.type, msg.data);
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
	
	/*Create semaphore*/
	if (!g_dmne_sem) {
		g_dmne_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
		if (!g_dmne_sem) {
			Print("Failed to create semaphore\n");
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

/* CLI: Send Event */
int os_demo_send_msg(int type, char *data)
{	
	message_t msg;

	msg.type = type;
	strncpy(msg.data, data, MAX_DATA_LEN - 1);
	
    if (msgQSend(g_msgQ_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) == 0) {
		semGive(g_dmne_sem);
        printf("Task tty0, sent a message: type %d, data %s\n", msg.type, msg.data);
        return 0;
    } else {
        printf("Failed to send message to DMNE\n");
        return -1;
    }
}

/* CLI: Send Event */
int os_demo_send_event(UINT32 mask)
{
    UINT32 tid = taskNameToId(DMNE_TASK_NAME);

    if (tid == 0)
    {
        Print("DMNE task not found\n");
        return -1;
    }

    if (eventSend(tid, mask) == OK)
    {
		semGive(g_dmne_sem);
        Print("Task tty0, sent an event 0x%X to the receiving task\n", mask);
        return 0;
    }
    else
    {
        Print("Failed to send event\n");
        return -1;
    }
}

/* Init function - called at boot */
void os_demo_init(void)
{	
	//##########################________________print_on_boot______________###################
	Print("_______________Hello World_______________\n");
	
	os_demo_register_cmds();
}
