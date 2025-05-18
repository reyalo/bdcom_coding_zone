// os-demo-cmd-impl.c
#include <vxWorks.h>
#include <taskLib.h>
#include <sysLib.h>

extern int g_int_data;

void os_demo_cmd_impl_test1_process(void)
{
	int i;
    TASK_ID tid = (TASK_ID)taskIdSelf();
    char *name = (char *)taskName((int)tid);
	
	

    for (i = 0; i < 30; i++)
    {
        g_int_data++;
        Print("Tick %d: Task Name: %s | Task ID: 0x%X | g_int_data: %d\n",
              i + 1, name, (unsigned int)tid, g_int_data);
        taskDelay(sysClkRateGet());  // delay 1 second
    }
}

void os_demo_cmd_impl_test1(void)
//int os_demo_cmd_impl_test1_wrapper(int argc, char* argv[])
{
    TASK_ID tid = taskSpawn("DM2", 128, 0, 1024,
                            (FUNCPTR)os_demo_cmd_impl_test1_process,
                            0,0,0,0,0,0,0,0,0,0);

    if (tid == (TASK_ID)-1)
    {
        Print("Failed to create task DM2\n");
        return ;
    }

    char *name = (char *)taskName((int)tid);
    Print("Task Created: Name = %s | ID = 0x%X\n", name, (unsigned int)tid);
	
	return;
}
