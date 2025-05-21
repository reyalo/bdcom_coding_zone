#include "os-demo.h"

void os_demo_cmd_impl_send_msg(int type, int value)
{
	int rv;
	
	struct demo_msg msg;
	msg.msg_type = type;
	msg.data = value;
	
	rv = msgQSend(msgq_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL);
	if (rv == ERROR)
		Print("Failed to send a message\n");
	else
		Print("Task %s, Sent a message: type %u, data %u\n", taskName(taskIdSelf()), type, value);
	
	return;
}

void os_demo_cmd_impl_send_event(int event)
{
	int rv;
	
	rv = eventSend(event_task_id, event);
	if (rv == ERROR)
		Print("Failed to send an event\n");
	else
		Print("Task %s, Sent an event 0x%x to the receiving task\n", taskName(taskIdSelf()), event);
}
