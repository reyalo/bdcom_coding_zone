#include <libcmd/cmdparse.h>
#include <libcmd/argparse.h>
#include <libcmd/cmderror.h>

void os_demo_register_cmds(void);
static int top_os_demo_config(int argc, char **argv, struct user *u);
static int second_os_demo_config_test1(int argc, char **argv, struct user *u);
static int second_os_demo_config_msg(int argc, char **argv, struct user *u);
static int second_os_demo_config_event(int argc, char **argv, struct user *u);
static int second_os_demo_config_test3(int argc, char **argv, struct user *u);
void os_demo_cmd_impl_send_msg(int type, int value);
void os_demo_cmd_impl_send_event(int event);

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
	{ "msg", MATCH_AMB, cmdPref(PF_CMDNO, PF_CMDDEF, 0), 0,
		second_os_demo_config_msg, NULL, NULL, 2, 0,
		"msg       -- msg",
		"msg       -- msg",
		NULLCHAR, NULLCHAR
	},
	{ "event", MATCH_AMB, cmdPref(PF_CMDNO, PF_CMDDEF, 0), 0,
		second_os_demo_config_event, NULL, NULL, 2, 0,
		"event       -- event",
		"event       -- event",
		NULLCHAR, NULLCHAR
	},
	{ "A.B.C.D", MATCH_ADD, cmdPref(PF_CMDNO, 0, 0), 0, 
		second_os_demo_config_test3, NULL, NULL, 0, 0,
		"A.B.C.D     -- IP address of host ",
		"A.B.C.D     -- IP address of host ",
		NULLCHAR, NULLCHAR },
	{ NULLCHAR }
};

static int second_os_demo_config_test1(int argc, char *argv[], struct user *u)
{
	int rv;

	if( (rv = cmdend(argc - 1, argv + 1, u)) )
		return rv;

	printf("%s,%d\n", __FUNCTION__, __LINE__);
	os_demo_cmd_impl_get_counter();

	return 0;
}

static int second_os_demo_config_msg(int argc, char **argv, struct user *u)
{
	int error, type, value;
	struct parameter param;
	
	memset(&param, 0, sizeof(param));
	param.type = ARG_INT;
	param.min = 1;
	param.max = 5;
	param.ylabel = "  <1-5>	  	-- msg type\n";
	param.hlabel = "  <1-5>	  	-- msg type\n";
	param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;
	if ((error = getparameter(argc, argv, u, &param)))
		return error;
	
	type = param.value.v_int;
	
	argc--;
	argv++;

	memset(&param, 0, sizeof(param));
	param.type = ARG_INT;
	param.min = 1;
	param.max = 100;
	param.ylabel = "  <1-100>	  -- msg value\n";
	param.hlabel = "  <1-100>	  -- msg value\n";
	param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;
	if ((error = getparameter(argc, argv, u, &param)))
		return error;
	
	value = param.value.v_int;
	
	argc--;
	argv++;
	
	if((error = cmdend(argc - 1, argv + 1, u)))
		return error;
	
	return 0;
}

static int second_os_demo_config_event(int argc, char **argv, struct user *u)
{
	int error, event;
	struct parameter param;
	
	memset(&param, 0, sizeof(param));
	param.type = ARG_INT;
	param.min = 1;
	param.max = 10;
	param.ylabel = "  <1-10>	  	-- event\n";
	param.hlabel = "  <1-10>	  	-- event\n";
	param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;
	if ((error = getparameter(argc, argv, u, &param)))
		return error;
	
	event = param.value.v_int;
	
	argc--;
	argv++;
	
	if((error = cmdend(argc - 1, argv + 1, u)))
		return error;
	
	return 0;
}

static int second_os_demo_config_test3(int argc, char **argv, struct user *u)
{
	int error, ret = 0;
	unsigned long addr = 0;
	
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
