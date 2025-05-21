#include "os-demo.h"

MSG_Q_ID os_demo_msgq_id;
int os_demo_task_id;
SEM_ID g_os_demo_sem;

void os_demo_main_task_init(void);
void os_demo_msg_event_recv_1(void);
void os_demo_msg_event_recv_2(void);
void os_demo_msg_event_recv_3(void);
void os_demo_msg_event_recv_4(void);
void os_demo_register_cmds(void);

void os_demo_init(void)
{
	os_demo_register_cmds();
	
	os_demo_main_task_init();
	
	return;
}

void os_demo_main_task_init(void)
{
	g_os_demo_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	if (g_os_demo_sem == NULL)
	{
		Print("Failed to create semaphore\n");
		return;
	}
		
	os_demo_msgq_id = msgQCreate(DEMO_MSG_LEN, sizeof(struct demo_msg), MSG_Q_FIFO);
	if (os_demo_msgq_id == NULL)
	{
		Print("Failed to create message queue\n");
		return;
	}
	
	os_demo_task_id = taskSpawn("DMNE", TASK_PRIORITY, 0, STACK_SIZE, (FUNCPTR)os_demo_msg_event_recv_3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (os_demo_task_id == ERROR)
		Print("Failed to create mesage and event receiving task\n");
	
	return;
}

void os_demo_msg_event_recv_1(void)
{
	int rv;
	UINT32 events;
	struct demo_msg msg;
	
	while(1)
	{
		events = 0;
		rv = eventReceive(DEMO_EVENT_MASK, EVENTS_WAIT_ANY, WAIT_FOREVER, &events);
		if (rv == ERROR)
		{
			Print("Failed to receive event\n");
			continue;
		}
		
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
		
		if (events & DEMO_EVENT_MSG)
		{
			while(1)
			{
				rv = msgQReceive(os_demo_msgq_id, (char *)&msg, sizeof(msg), NO_WAIT);
				if (rv == ERROR)
				{
					break;
				}
				
				Print("Task %s, Received a message: type %u, data %u\n", taskName(taskIdSelf()), msg.msg_type, msg.data);
			}
		}
	}
	
	return;
}

void os_demo_msg_event_recv_2(void)
{
	int rv;
	UINT32 events;
	struct demo_msg msg;
	
	while(1)
	{
		rv = msgQReceive(os_demo_msgq_id, (char *)&msg, sizeof(msg), WAIT_FOREVER);
		if (rv == ERROR)
		{
			Print("Failed to receive message\n");
			continue;
		}
		
		Print("Task %s, Received a message: type %u, data %u\n", taskName(taskIdSelf()), msg.msg_type, msg.data);
		
		if (msg.msg_type == DEMO_MSG_TYPE_EVENT)
		{
			events = 0;
			rv = eventReceive(DEMO_EVENT_MASK, EVENTS_WAIT_ANY, NO_WAIT, &events);
			if (rv == ERROR)
			{
				continue;
			}
			
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
	}
	
	return;
}

void os_demo_msg_event_recv_3(void)
{
	int rv;
	UINT32 events;
	struct demo_msg msg;
	
	while(1)
	{
		semTake(g_os_demo_sem, WAIT_FOREVER);
		
		rv = msgQReceive(os_demo_msgq_id, (char *)&msg, sizeof(msg), NO_WAIT);
		if (rv != ERROR)
		{
			Print("Task %s, Received a message: type %u, data %u\n", taskName(taskIdSelf()), msg.msg_type, msg.data);
		}
		
		events = 0;
		rv = eventReceive(DEMO_EVENT_MASK, EVENTS_WAIT_ANY, NO_WAIT, &events);
		if (rv != ERROR)
		{
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
	}
	
	return;
}