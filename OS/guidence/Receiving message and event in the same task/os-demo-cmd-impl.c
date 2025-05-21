#include "os-demo.h"

void os_demo_cmd_impl_send_msg_evt_1(int type, int value)
{
	int rv;
	
	struct demo_msg msg;
	msg.msg_type = type;
	msg.data = value;
	
	rv = msgQSend(os_demo_msgq_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL);
	if (rv == ERROR)
	{
		Print("Failed to send a message\n");
	}
	else
	{
		// After sending a message, send an event to the receiving task immediately to notify it of the message
		rv = eventSend(os_demo_task_id, DEMO_EVENT_MSG);
		if (rv == ERROR)
			Print("Failed to send an event for the message\n");
		else
			Print("Task %s, Sent an event for message: type %u, data %u\n", taskName(taskIdSelf()), type, value);
	}
	
	return;
}

void os_demo_cmd_impl_send_msg_evt_2(int type, int event)
{
	int rv;
	struct demo_msg msg;
	
	rv = eventSend(os_demo_task_id, event);
	if (rv == ERROR)
	{
		Print("Failed to send an event\n");
	}
	else
	{
		/* After sending an event, send a message to the receiving task 
		  immediately to notify it of the event. */
		msg.msg_type = DEMO_MSG_TYPE_EVENT;
		msg.data = type;
		rv = msgQSend(os_demo_msgq_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL);
		if (rv == ERROR)
			Print("Failed to send a message for the event\n");
		else
			Print("Task %s, Sent an event 0x%x to the receiving task\n", taskName(taskIdSelf()), event);
	}
}

void os_demo_cmd_impl_send_msg_evt_3(int type, int event)
{
	int rv;
	struct demo_msg msg;
	
	rv = eventSend(os_demo_task_id, event);
	if (rv == ERROR)
	{
		Print("Failed to send an event\n");
	}
	else
	{
		semGive(g_os_demo_sem);
		Print("Task %s, Sent an event 0x%x to the receiving task\n", taskName(taskIdSelf()), event);
	}
}