#include <libcmd/cmdparse.h>
#include <libcmd/argparse.h>
#include <libcmd/cmderror.h>

void os_demo_register_cmds(void);
static int top_os_demo_config(int argc, char **argv, struct user *u);
static int second_os_demo_config_test1(int argc, char **argv, struct user *u);
static int second_os_demo_config_test2(int argc, char **argv, struct user *u);
static int second_os_demo_config_test3(int argc, char **argv, struct user *u);
extern void os_demo_cmd_impl_test1(void);

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
		NULLCHAR, NULLCHAR },
	{ NULLCHAR }
};

static int second_os_demo_config_test1(int argc, char *argv[], struct user *u)
{
	int rv;

	if( (rv = cmdend(argc - 1, argv + 1, u)) )
		return rv;

	printf("%s,%d\n", __FUNCTION__, __LINE__);
	os_demo_cmd_impl_test1();

	return 0;
}

static int second_os_demo_config_test2(int argc, char **argv, struct user *u)
{
	int error;
	unsigned long value;

	switch(getuint(&value, argc, argv, u)) 
	{
		case ARG_NOERR:
			break;
		case ARG_QPART:
		case ARG_QUERY:
			eng_chi (u, "  value          -- value",
						"  value          -- value");
			return 1;
		default:
			return -1;
	}
	
	argc--;
	argv++;
	
	if((error = cmdend(argc - 1, argv + 1, u)))
		return error;
	
	taskDelete(value);
	
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
