#include <libcmd/cmdparse.h>
#include <libcmd/argparse.h>
#include <libcmd/cmderror.h>

void os_demo_register_cmds(void);
static int top_os_demo_config(int argc, char **argv, struct user *u);
static int second_os_demo_config_test1(int argc, char **argv, struct user *u);
static int second_os_demo_config_test2(int argc, char **argv, struct user *u);
static int second_os_demo_config_test3(int argc, char **argv, struct user *u);
static int second_os_demo_config_msg_event(int argc, char **argv, struct user *u);
static int second_os_demo_config_msg(int argc, char **argv, struct user *u);
static int second_os_demo_config_event(int argc, char **argv, struct user *u);

struct topcmds top_os_demo_cmd[] = {
	{ "os-demo", cmdPref(PF_CMDNO, PF_CMDDEF, 0), IF_NULL, ~FG_GLOBAL, IF_NULL, FG_ENABLE, 
		top_os_demo_config, NULL, NULL, 0, 0,
		"os-demo        -- os-demo",
		"os-demo        -- os-demo",
		NULLCHAR, NULLCHAR
	},
	{ NULLCHAR }
};



struct cmds second_os_demo_cmd[] = {
	{ "test1", MATCH_AMB, 0, 0,
		second_os_demo_config_test1, NULL, NULL, 0, 0,
		"test1        -- test1",
		"test1        -- test1",
		NULLCHAR, NULLCHAR
	},
	{ "test2", MATCH_AMB, cmdPref(PF_CMDNO, PF_CMDDEF, 0), 0,
		second_os_demo_config_test2, NULL, NULL, 2, 2,
		"test2       -- test2",
		"test2       -- test2",
		NULLCHAR, NULLCHAR
	},
	{ "A.B.C.D", MATCH_ADD, cmdPref(PF_CMDNO, 0, 0), 0, 
		second_os_demo_config_test3, NULL, NULL, 0, 0,
		"A.B.C.D     -- IP address of host ",
		"A.B.C.D     -- IP address of host ",
		NULLCHAR, NULLCHAR 
	},
	{
        "msgevent", MATCH_AMB, 0, 0,
        second_os_demo_config_msg_event, NULL, NULL, 0, 0,
        "msgevent         -- Create message/event tasks",
        "msgevent         -- Create message/event tasks",
        NULLCHAR, NULLCHAR
    },
	{
        "msg", MATCH_AMB, 0, 0,
        second_os_demo_config_msg, NULL, NULL, 0, 0,
        "msg <type> <data>        -- Send message to DMSG",
        "msg <type> <data>        -- Send message to DMSG",
        NULLCHAR, NULLCHAR
    },
    {
        "event", MATCH_AMB, 0, 0,
        second_os_demo_config_event, NULL, NULL, 0, 0,
        "event <bitmask>          -- Send event to DEVT",
        "event <bitmask>          -- Send event to DEVT",
        NULLCHAR, NULLCHAR
    },
	{ NULLCHAR }
};

/*
struct cmds second_os_demo_cmd[] = {
    {
        "msg", MATCH_AMB, 0, 0,
        os_demo_send_msg, NULL, NULL, 0, 0,
        "msg <type> <data>        -- Send message to DMSG",
        "msg <type> <data>        -- Send message to DMSG",
        NULLCHAR, NULLCHAR
    },
    {
        "event", MATCH_AMB, 0, 0,
        os_demo_send_event, NULL, NULL, 0, 0,
        "event <bitmask>          -- Send event to DEVT",
        "event <bitmask>          -- Send event to DEVT",
        NULLCHAR, NULLCHAR
    },
    { NULLCHAR }
};

*/


// #################################________msg_event_task_create___________________________________

static int second_os_demo_config_msg_event(int argc, char *argv[], struct user *u)
{
	
	os_demo_create_msg_event_tasks();
	printf("%s,%d\n", __FUNCTION__, __LINE__);

	return 0;
}

// #################################________msg___________________________________

static int second_os_demo_config_msg(int argc, char *argv[], struct user *u)
{
	
	//os_demo_send_msg();
    return os_demo_send_msg(argc, argv);

	printf("%s,%d\n", __FUNCTION__, __LINE__);

	return 0;
}

// #################################__________event________________________________

static int second_os_demo_config_event(int argc, char *argv[], struct user *u)
{
	
	return os_demo_send_event(argc, argv);
	printf("%s,%d\n", __FUNCTION__, __LINE__);

	return 0;
}



// #################################################################################
static int second_os_demo_config_test2(int argc, char **argv, struct user *u)
{
	int error;
    struct parameter param;

    memset(&param, 0, sizeof(param));
    param.type = ARG_STR;
    param.min = 1;
    param.max = 10;
    param.ylabel = "WORD         -- task name (e.g., DM2)\n";
    param.hlabel = "WORD         -- task name (e.g., DM2)\n";
    param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;

    if ((error = getparameter(argc, argv, u, &param)))
        return error;

    argc--;
    argv++;

    if ((error = cmdend(argc - 1, argv + 1, u)))
        return error;

    // Get the string and construct the task name
    char *numStr = param.value.v_string;
    char taskNameBuf[16] = "DM";
    strcat(taskNameBuf, numStr);  // DM + number

    // Get task ID
    int tid = taskNameToId(taskNameBuf);
    if (tid == ERROR) {
        Print("Error: No task found with name '%s'\n", taskNameBuf, 0, 0, 0, 0, 0, 0);
        return -1;
    }

    // Delete the task
    if (taskDelete(tid) == OK) {
        Print("Success: Deleted task '%s' (ID = 0x%X)\n", taskNameBuf, tid, 0, 0, 0, 0, 0);
    } else {
        Print("Error: Failed to delete task '%s' (ID = 0x%X)\n", taskNameBuf, tid, 0, 0, 0, 0, 0);
    }
	
	return 0;
}


static int second_os_demo_config_test1(int argc, char *argv[], struct user *u)
{
	
	//os_demo_cmd_impl_test1();
	printf("%s,%d\n", __FUNCTION__, __LINE__);

	return 0;
}


static int second_os_demo_config_test3(int argc, char **argv, struct user *u)
{
	int error, ret = 0;
	unsigned int addr = 0;
	
	if((error = cmdend(argc - 1, argv + 1, u)))
		return error;
	
	if(ARG_NOERR != (ret = getaddress(&addr, argc + 1, argv - 1, u)))
		return ret;
	
	printf("%x\n", addr);
	
	return 0;
}

static int top_os_demo_config(int argc, char **argv, struct user *u)
{
	return subcmd(second_os_demo_cmd, NULL, argc, argv, u);
}

void os_demo_register_cmds(void)
{
	registercmd(top_os_demo_cmd);
}
