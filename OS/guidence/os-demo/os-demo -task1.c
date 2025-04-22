#include <vxWorks.h>
#include <taskLib.h>
#include <logLib.h>
#include <stdio.h>
#include <sysLib.h>
#include <unistd.h>  // for sleep()


//void os_demo_cmd_impl_test1_process(void);

// Declare DM2 wrapper (defined in other file)
int os_demo_cmd_impl_test1_wrapper(int argc, char* argv[]);


/* Global variable */
int g_int_data = 0;



/* Task function */
void os_demo_main_process(void)
{
    int i;
    TASK_ID tid = (TASK_ID)taskIdSelf();
    //char taskName[32];

    //taskName(tid, taskName);  // Get the task name
	
	char* name = (char *)taskName(taskIdSelf());


    for (i = 0; i < 10; i++)
    {
        g_int_data++;
        Print("Tick %d: Task Name: %s | Task ID: 0x%X | g_int_data: %d\n",
               i + 1, name, (unsigned int)tid, g_int_data);
		
		//logMsg("Tick %d: Task Name: %s | Task ID: 0x%X | g_int_data: %d\n",
       //i + 1, (int)name, (unsigned int)tid, g_int_data, 0, 0);

        //taskDelay(sysClkRateGet()); // delay 1 second
		sleep(1);
    }
}

/* Init function - called at boot */
void os_demo_init(void)
{
	logMsg("Hello World\n",0,0,0,0,0,0);
	
    TASK_ID tid;

    tid = (TASK_ID)taskSpawn("DM1",        // Task name
                    128,          // Priority
                    0,            // Options
                    1024,         // Stack size
                    (FUNCPTR) os_demo_main_process,  // Entry point
                    0,0,0,0,0,0,0,0,0,0); // No args

    if (tid == (TASK_ID)-1)
    {
        //logMsg("Failed to create task DM1.\n",0,0,0,0,0,0);
		Print("Failed to create task DM1.\n");
    }
    else
    {
        Print("Task Created: Name = DM1 | ID = 0x%X\n", (unsigned int)tid);
    }
	
	os_demo_register_cmds();
}
