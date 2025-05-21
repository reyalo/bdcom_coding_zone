#include "os-demo.h"

void os_demo_cmd_impl_send_msg_evt(int type, int event)
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