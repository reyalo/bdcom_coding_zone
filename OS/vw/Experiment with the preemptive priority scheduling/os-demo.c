#include <stdlib.h>
#include <taskLib.h>

#define MAX_TEST_COUNT_VALUE 640000
#define MIN_COUNT_INTERVAL 1
#define MAX_COUNT_INTERVAL 10 

extern unsigned long Print(char *format, ...);
void os_demo_normal_prio_count();
void os_demo_normal_prio_check();
void os_demo_high_prio_check();

volatile int test_count;

void os_demo_init()
{
	int task_id;

	task_id = taskSpawn("DNCN", 128, 0, 1024, (FUNCPTR)os_demo_normal_prio_count, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (task_id == ERROR)
		Print("Failed to create task Count\n");

	task_id = taskSpawn("DNCK", 128, 0, 1024, (FUNCPTR)os_demo_normal_prio_check, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (task_id == ERROR)
		Print("Failed to create task Check with normal priority\n");

	task_id = taskSpawn("DHCK", 126, 0, 1024, (FUNCPTR)os_demo_high_prio_check, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (task_id == ERROR)
		Print("Failed to create task Check with higher priority\n");
 
	return;
}

void os_demo_normal_prio_count()
{
	int task_id;
	char *task_name;
	int test_data;
	int i;
	
	task_id = taskIdSelf();
	task_name = taskName(task_id);
	Print("Task ID: 0x%x, task name: %s\n", task_id, task_name);
	
	while(1)
	{
		/* A non-blocking code block. An other task with the same priority cannot 
	       preempt CPU from me during this period but a task with higher priority
	       can. */
		for (test_count = 0; test_count < MAX_TEST_COUNT_VALUE; test_count++)
		{
			for (i = 0; i < 1000; i++)
				test_data = i;
		}

		/* Call taskDelay() actively to yield CPU and enter the 
		   above non-blocking code block again at a random time. */
		taskDelay(MIN_COUNT_INTERVAL + rand() % MAX_COUNT_INTERVAL);
	}

	return;
}

void os_demo_normal_prio_check()
{
	int task_id;
	char *task_name;
	
	task_id = taskIdSelf();
	task_name = taskName(task_id);
	Print("Task ID: 0x%x, task name: %s\n", task_id, task_name);
	
	while(1)
	{
		Print("Checking test_count in normal priority task: %d\n", test_count);
		taskDelay(167); /* Avoid waking up at the same time as task DHCK */
	}

	return;
}

void os_demo_high_prio_check()
{
	int task_id;
	char *task_name;
	
	task_id = taskIdSelf();
	task_name = taskName(task_id);
	Print("Task ID: 0x%x, task name: %s\n", task_id, task_name);
	
	while(1)
	{
		Print("Checking test_count in high priority task: %d\n", test_count);
		taskDelay(512); /* Avoid waking up at the same time as task DNCK */
	}

	return;
}

