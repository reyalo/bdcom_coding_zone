#include <libcmd/cmdparse.h>
#include <libcmd/argparse.h>
#include <libcmd/cmderror.h>

void os_demo_register_cmds(void);
static int top_os_demo_config(int argc, char **argv, struct user *u);
static int second_os_demo_config_test1(int argc, char **argv, struct user *u);
static int second_os_demo_config_msg_evt(int argc, char **argv, struct user *u);
static int second_os_demo_config_test3(int argc, char **argv, struct user *u);
void os_demo_cmd_impl_send_msg_evt_1(int msg, int value);
void os_demo_cmd_impl_send_msg_evt_2(int msg, int value);
void os_demo_cmd_impl_send_msg_evt_3(int msg, int value);

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
	{ "msg-evt", MATCH_AMB, cmdPref(PF_CMDNO, PF_CMDDEF, 0), 0,
		second_os_demo_config_msg_evt, NULL, NULL, 2, 0,
		"msg-evt       -- msg-evt",
		"msg-evt       -- msg-evt",
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

	return 0;
}

static int second_os_demo_config_msg_evt(int argc, char **argv, struct user *u)
{
	int error, msgv, evtv;
	struct parameter param;
	
	memset(&param, 0, sizeof(param));
	param.type = ARG_INT;
	param.min = 1;
	param.max = 5;
	param.ylabel = "  <1-5>	  	-- msg\n";
	param.hlabel = "  <1-5>	  	-- msg\n";
	param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;
	if ((error = getparameter(argc, argv, u, &param)))
		return error;
	
	msgv = param.value.v_int;
	
	argc--;
	argv++;

	memset(&param, 0, sizeof(param));
	param.type = ARG_INT;
	param.min = 1;
	param.max = 32;
	param.ylabel = "  <1-32>	  -- event\n";
	param.hlabel = "  <1-32>	  -- event\n";
	param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;
	if ((error = getparameter(argc, argv, u, &param)))
		return error;
	
	evtv = param.value.v_int;
	
	argc--;
	argv++;
	
	if ((error = cmdend(argc - 1, argv + 1, u)))
		return error;
	
	if (msgv == 1)
		os_demo_cmd_impl_send_msg_evt_1(msgv, evtv);
	else if(msgv == 2)
		os_demo_cmd_impl_send_msg_evt_2(msgv, evtv);
	else if(msgv == 3)
		os_demo_cmd_impl_send_msg_evt_3(msgv, evtv);
	
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
