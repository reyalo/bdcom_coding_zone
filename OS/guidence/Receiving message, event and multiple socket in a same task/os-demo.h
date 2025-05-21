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
	unsigned long sub_type;
};

enum msg_type
{
	DEMO_MSG_TYPE1 = 1,
	DEMO_MSG_TYPE2,
	DEMO_MSG_TYPE3,
	DEMO_MSG_TYPE_EVENT
};

#define DEMO_EVENT_A	0x00000001 
#define DEMO_EVENT_B    0x00000002
#define DEMO_EVENT_C    0x00000004
#define DEMO_EVENT_MSG  0x00000008
#define DEMO_EVENT_MASK 0x0000000F

#define DEMO_MSG_LEN 20

extern MSG_Q_ID os_demo_msgq_id;
extern int os_demo_task_id;

extern unsigned long Print(char *format, ...);

#endif
