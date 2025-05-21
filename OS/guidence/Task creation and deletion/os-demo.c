#include <taskLib.h>

#define TASK_PRIORITY 128
#define STACK_SIZE 1024
void os_demo_main_process(void);
void os_demo_register_cmds(void);
extern unsigned long Print(char *format, ...);
int g_int_data = 0;

void os_demo_init(void)
{
	os_demo_register_cmds();
	
	int task_id;
	
	task_id = taskIdSelf();
	Print("%s,%d, Task ID: 0x%x, Task name: %s\n", __FUNCTION__, __LINE__, task_id, taskName(task_id));

    task_id = taskSpawn(
        "DM1",      	/* name of new task (stored at pStackBase) */
        TASK_PRIORITY,	/* priority of new task */
        0,				/* task option word */
        STACK_SIZE,		/* size (bytes) of stack needed plus name */
        (FUNCPTR)os_demo_main_process,   /* entry point of new task */
        0,           	/* 1st of 10 req'd task args to pass to func */
        0, 0, 0, 0, 0, 0, 0, 0, 0 /* args 2~10*/
    );
    
    if (task_id == ERROR)
	{
        Print("Failed to create task\n");
		return;
	}
	
	Print("%s,%d, Task ID: 0x%x, Task name: %s\n", __FUNCTION__, __LINE__, task_id, taskName(task_id));
	
	return;
}

void os_demo_main_process(void)
{
    int task_id = taskIdSelf();
    
    while(1)
    {
		g_int_data++;
        Print("%s,%d, Task ID: 0x%x, Task name: %s\n", __FUNCTION__, __LINE__, task_id, taskName(task_id));
        taskDelay(10*100); /* 1 tick = 10ms */
    }

    return;
}
