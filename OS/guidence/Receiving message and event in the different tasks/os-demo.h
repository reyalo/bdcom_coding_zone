#ifndef __OS_DEMO_H_
#define __OS_DEMO_H_

#include <msgQLib.h>
#include <taskLib.h>
#include <eventLib.h>

struct demo_msg
{
	unsigned long msg_type;
	unsigned long data;
	unsigned long reserved1;
	unsigned long reserved2;
};

enum msg_type
{
	DEMO_MSG_TYPE1 = 1,
	DEMO_MSG_TYPE2,
	DEMO_MSG_TYPE3
};

#define DEMO_EVENT_A	0x00000001 /* bit 0, event TEST, used in this experiment */
#define DEMO_EVENT_B    0x00000002 /* bit 1, event A, just an example for demonstration */
#define DEMO_EVENT_C    0x00000004 /* bit 2, event B... */
#define DEMO_EVENT_MASK	0x00000007

#define DEMO_MSG_LEN	20
#define TASK_PRIORITY	128
#define STACK_SIZE		1024

extern MSG_Q_ID msgq_id;
extern int event_task_id;

extern unsigned long Print(char *format, ...);

#endif
