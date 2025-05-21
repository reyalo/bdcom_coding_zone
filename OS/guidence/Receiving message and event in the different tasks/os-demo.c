#include "os-demo.h"

MSG_Q_ID msgq_id;
int event_task_id;

void os_demo_main_task_init(void);
void os_demo_msg_recv(void);
void os_demo_event_recv(void);
void os_demo_register_cmds(void);

void os_demo_init(void)
{
	os_demo_register_cmds();
	
	os_demo_main_task_init();
	
	return;
}

void os_demo_main_task_init(void)
{
	int task_id;
	
	msgq_id = msgQCreate(DEMO_MSG_LEN, sizeof(struct demo_msg), MSG_Q_FIFO);
	if (msgq_id == NULL)
	{
		Print("Failed to create message queue\n");
		return;
	}
	
	task_id = taskSpawn("DMSG", TASK_PRIORITY, 0, STACK_SIZE, (FUNCPTR)os_demo_msg_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (task_id == ERROR)
		Print("Failed to create message receiving task\n");
	
	event_task_id = taskSpawn("DEVT", TASK_PRIORITY, 0, STACK_SIZE, (FUNCPTR)os_demo_event_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (event_task_id == ERROR)
		Print("Failed to create event receiving task\n");
	
	return;
}

void os_demo_msg_recv(void)
{
	int rv;
	struct demo_msg msg;
	
	while(1)
	{
		rv = msgQReceive(msgq_id, (char *)&msg, sizeof(msg), WAIT_FOREVER);
		if (rv == ERROR)
		{
			Print("Failed to receive message\n");
			sleep(1);	// Avoid to busy wait
			continue;
		}
		
		Print("Task %s, Received a message: type %u, data %u\n", taskName(taskIdSelf()), msg.msg_type, msg.data);
	}
	
	return;
}

void os_demo_event_recv(void)
{
	int rv;
	UINT32 events;
	
	while(1)
	{
		events = 0;
		/* Here 0xffffffff means I'm expecting all events, however we could use 
		   a set of events which I care about instead, such as DEMO_EVENT_MASK */
		rv = eventReceive(0xffffffff, EVENTS_WAIT_ANY, WAIT_FOREVER, &events);
		if (rv == ERROR)
		{
			Print("Failed to receive event\n");
			sleep(1);	// Avoid to busy wait
			continue;
		}
		
		/* Usually a task needs to handle the events it cares about one by one, 
		   so they must be picked out one by one. */
		if (events & DEMO_EVENT_A)
		{
			Print("Task %s, Received event 0x%x\n", taskName(taskIdSelf()), DEMO_EVENT_A);
			events &= ~DEMO_EVENT_A;
		}
		
		if (events & DEMO_EVENT_B)
		{
			Print("Task %s, Received event 0x%x\n", taskName(taskIdSelf()), DEMO_EVENT_B);
			events &= ~DEMO_EVENT_B;
		}
		
		if (events & DEMO_EVENT_C)
		{
			Print("Task %s, Received event 0x%x\n", taskName(taskIdSelf()), DEMO_EVENT_C);
			events &= ~DEMO_EVENT_C;
		}
	}
	
	return;
}
