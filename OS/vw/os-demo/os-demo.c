#include <vxWorks.h>
#include <taskLib.h>
#include <logLib.h>
#include <stdio.h>
#include <sysLib.h>
#include <unistd.h>  // for sleep()



//void os_demo_cmd_impl_test1_process(void);
int Print(const char *fmt, ...);

void os_demo_register_cmds(void);



/* Init function - called at boot */
void os_demo_init(void)
{
	Print("_______________Hello World_______________\n");
	
	os_demo_register_cmds();
}
