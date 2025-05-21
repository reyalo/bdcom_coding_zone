#include <taskLib.h>
#include <msgQLib.h>
#include <tickLib.h>

/* message type(s) */
#define DEMO_MSG_TYPE_TEST 1

#define TEST_TABLE_LEN (16*1024)
#define CPU_YIELD_INTERVAL 150 /* in unit of tick, i.e., 1.5 seconds */
#define CPU_YIELD_TIME 3 /* in unit of tick, i.e., 30 milliseconds */

/* In order to be compatible with the message format in Baud 
   switch software, we use a message struct consisting of 
   four unsigned long fields. */
typedef struct  
{
	unsigned long msg_type;
	unsigned long count;
	unsigned long reserved1;
	unsigned long reserved2;
} demo_msg_t;

extern unsigned long Print(char *format, ...);
void os_demo_send();
void os_demo_recv();

MSG_Q_ID msgq_id;
long test_table[TEST_TABLE_LEN];

void os_demo_init()
{
	int task_id;

	msgq_id = msgQCreate(256, sizeof(demo_msg_t), MSG_Q_FIFO);
	if (msgq_id == NULL)
	{
		Print("Failed to create message queue\n");
		return;
	}

	task_id = taskSpawn("DMRV", 128, 0, 1024, (FUNCPTR)os_demo_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (task_id == ERROR)
		Print("Failed to create receiving task\n");
	
	task_id = taskSpawn("DMSD", 128, 0, 1024, (FUNCPTR)os_demo_send, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (task_id == ERROR)
		Print("Failed to create sending task\n");
 
	return;
}

void os_demo_send()
{
	static unsigned int count = 0;
	demo_msg_t msg = {.msg_type = DEMO_MSG_TYPE_TEST};
	int rv;

	while(1)
	{
		msg.count = ++count;
		rv = msgQSend(msgq_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL);
		if (rv == ERROR)
			Print("Failed to send a message\n");
		else
			Print("Sent a message: type %u, count %u\n", msg.msg_type, msg.count);

		taskDelay(10*100);
	}

	return;
}

void os_demo_recv()
{
	demo_msg_t msg;
	int rv;
	long i, j;
	ULONG timestamp;
	
	while(1)
	{
		rv = msgQReceive(msgq_id, (char *)&msg, sizeof(msg), WAIT_FOREVER);
		if (rv == ERROR)
		{
			Print("Failed to receive message\n");
			continue;
		}

		switch(msg.msg_type)
		{
			case DEMO_MSG_TYPE_TEST:
				Print("Received a message: type %u, count %u\n", DEMO_MSG_TYPE_TEST, msg.count);

				/* Simulate too heavy work for processing message DEMO_MSG_TYPE_TEST here */
				timestamp = tickGet();
				for (i = 0; i < TEST_TABLE_LEN; i++)
				{
					for (j = 0; j < (1 << 20); j++)
						test_table[i] = i;

					if (tickGet() - timestamp >= CPU_YIELD_INTERVAL)
					{
						taskDelay(CPU_YIELD_TIME);
						timestamp = tickGet();
					}
				}

				Print("Work done\n");
				break;
			default:
				break;
		}
	}

	return;
}

