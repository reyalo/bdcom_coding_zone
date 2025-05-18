#include <libcmd/cmdparse.h>
#include <libcmd/argparse.h>
#include <libcmd/cmderror.h>
#include <stdlib.h> // Added for atoi

// External function declarations
void os_demo_register_cmds(void);
void os_demo_create_socket_task(void);
void os_demo_partition_alloc_handler(void);
void os_demo_partition_free_handler(void);
void os_demo_region_alloc_handler(size_t size);
void os_demo_region_free_handler(void);
void os_demo_region_realloc_handler(size_t size);
void os_demo_std_region_alloc_handler(size_t size);
void os_demo_std_region_free_handler(void);
void os_demo_std_region_realloc_handler(size_t size);

// Partition memory function declarations
static int os_demo_partition_alloc(int argc, char **argv, struct user *u);
static int os_demo_partition_free(int argc, char **argv, struct user *u);

// Region memory function declarations
static int os_demo_region_alloc(int argc, char **argv, struct user *u);
static int os_demo_region_free(int argc, char **argv, struct user *u);
static int os_demo_region_realloc(int argc, char **argv, struct user *u);
static int os_demo_std_region_alloc(int argc, char **argv, struct user *u);
static int os_demo_std_region_free(int argc, char **argv, struct user *u);
static int os_demo_std_region_realloc(int argc, char **argv, struct user *u);

// Subcommand dispatchers
static int top_os_demo_config(int argc, char **argv, struct user *u);
static int second_os_demo_config_partition(int argc, char **argv, struct user *u);
static int second_os_demo_config_region(int argc, char **argv, struct user *u);
static int second_os_demo_config_test1(int argc, char **argv, struct user *u);
static int second_os_demo_config_test2(int argc, char **argv, struct user *u);
static int second_os_demo_config_test3(int argc, char **argv, struct user *u);
static int second_os_demo_config_msg_event(int argc, char **argv, struct user *u);
static int second_os_demo_config_msg(int argc, char **argv, struct user *u);
static int second_os_demo_config_event(int argc, char **argv, struct user *u);

// Top-level command
struct topcmds top_os_demo_cmd[] = {
    { "os-demo", cmdPref(PF_CMDNO, PF_CMDDEF, 0), IF_NULL, ~FG_GLOBAL, IF_NULL, FG_ENABLE,
        top_os_demo_config, NULL, NULL, 0, 0,
        "os-demo        -- os-demo",
        "os-demo        -- os-demo",
        NULLCHAR, NULLCHAR
    },
    { NULLCHAR }
};

// Partition subcommands
static struct cmds partition_cmds[] = {
    { "alloc", MATCH_AMB, 0, 0,
        os_demo_partition_alloc, NULL, NULL, 0, 0,
        "alloc       -- Allocate partition memory",
        "alloc       -- Allocate partition memory",
        NULLCHAR, NULLCHAR
    },
    { "free", MATCH_AMB, 0, 0,
        os_demo_partition_free, NULL, NULL, 0, 0,
        "free        -- Free partition memory",
        "free        -- Free partition memory",
        NULLCHAR, NULLCHAR
    },
    { NULLCHAR }
};

// Region subcommands
static struct cmds region_cmds[] = {
    { "alloc", MATCH_AMB, 0, 0,
        os_demo_region_alloc, NULL, NULL, 0, 0,  // Changed from 1, 1 to 0, 0
        "alloc <size> -- Allocate region memory",
        "alloc <size> -- Allocate region memory",
        NULLCHAR, NULLCHAR
    },
    { "free", MATCH_AMB, 0, 0,
        os_demo_region_free, NULL, NULL, 0, 0,  // Already correct
        "free        -- Free region memory",
        "free        -- Free region memory",
        NULLCHAR, NULLCHAR
    },
    { "realloc", MATCH_AMB, 0, 0,
        os_demo_region_realloc, NULL, NULL, 0, 0,  // Changed from 1, 1 to 0, 0
        "realloc <size> -- Reallocate region memory",
        "realloc <size> -- Reallocate region memory",
        NULLCHAR, NULLCHAR
    },
    { "std-alloc", MATCH_AMB, 0, 0,
        os_demo_std_region_alloc, NULL, NULL, 0, 0,  // Changed from 1, 1 to 0, 0
        "std-alloc <size> -- Allocate region memory (std lib)",
        "std-alloc <size> -- Allocate region memory (std lib)",
        NULLCHAR, NULLCHAR
    },
    { "std-free", MATCH_AMB, 0, 0,
        os_demo_std_region_free, NULL, NULL, 0, 0,  // Already correct
        "std-free        -- Free region memory (std lib)",
        "std-free        -- Free region memory (std lib)",
        NULLCHAR, NULLCHAR
    },
    { "std-realloc", MATCH_AMB, 0, 0,
        os_demo_std_region_realloc, NULL, NULL, 0, 0,  // Changed from 1, 1 to 0, 0
        "std-realloc <size> -- Reallocate region memory (std lib)",
        "std-realloc <size> -- Reallocate region memory (std lib)",
        NULLCHAR, NULLCHAR
    },
    { NULLCHAR }
};

// Second-level os-demo subcommands
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
    { "msgevent", MATCH_AMB, 0, 0,
        second_os_demo_config_msg_event, NULL, NULL, 0, 0,
        "msgevent         -- Create message/event tasks",
        "msgevent         -- Create message/event tasks",
        NULLCHAR, NULLCHAR
    },
    { "msg", MATCH_AMB, 0, 0,
        second_os_demo_config_msg, NULL, NULL, 0, 0,
        "msg <type> <data>        -- Send message to DMSG",
        "msg <type> <data>        -- Send message to DMSG",
        NULLCHAR, NULLCHAR
    },
    { "event", MATCH_AMB, 0, 0,
        second_os_demo_config_event, NULL, NULL, 0, 0,
        "event <bitmask>          -- Send event to DEVT",
        "event <bitmask>          -- Send event to DEVT",
        NULLCHAR, NULLCHAR
    },
    { "partition", MATCH_AMB, 0, 0,
        second_os_demo_config_partition, NULL, NULL, 0, 0,
        "partition <alloc|free>  -- Partition memory operations",
        "partition <alloc|free>  -- Partition memory operations",
        NULLCHAR, NULLCHAR
    },
    { "region", MATCH_AMB, 0, 0,
        second_os_demo_config_region, NULL, NULL, 0, 0,
        "region <alloc|free|realloc|std-alloc|std-free|std-realloc>  -- Region memory operations",
        "region <alloc|free|realloc|std-alloc|std-free|std-realloc>  -- Region memory operations",
        NULLCHAR, NULLCHAR
    },
    { NULLCHAR }
};

// ############################## Command Implementations ##############################

static int second_os_demo_config_partition(int argc, char **argv, struct user *u)
{
    return subcmd(partition_cmds, NULL, argc, argv, u);
}

static int second_os_demo_config_region(int argc, char **argv, struct user *u)
{
    return subcmd(region_cmds, NULL, argc, argv, u);
}

static int os_demo_partition_alloc(int argc, char **argv, struct user *u)
{
    os_demo_partition_alloc_handler();
    Print("Partition memory block allocated.\n", 0, 0, 0, 0, 0, 0, 0);
    return 0;
}

static int os_demo_partition_free(int argc, char **argv, struct user *u)
{
    os_demo_partition_free_handler();
    Print("Partition memory block freed.\n", 0, 0, 0, 0, 0, 0, 0);
    return 0;
}

static int os_demo_region_alloc(int argc, char **argv, struct user *u)
{
    struct parameter param;
    int error;

    memset(&param, 0, sizeof(param));
    param.type = ARG_INT;
    param.min = 1;
    param.max = 1000000; // Arbitrary max size
    param.ylabel = "SIZE         -- Size in bytes\n";
    param.hlabel = "SIZE         -- Size in bytes\n";
    param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;

    if ((error = getparameter(argc, argv, u, &param))) {
        Print("[os-demo] Invalid size parameter.\n", 0, 0, 0, 0, 0, 0, 0);
        return error;
    }

    size_t size = param.value.v_int;
    os_demo_region_alloc_handler(size);
    return 0;
}

static int os_demo_region_free(int argc, char **argv, struct user *u)
{
    os_demo_region_free_handler();
    return 0;
}

static int os_demo_region_realloc(int argc, char **argv, struct user *u)
{
    struct parameter param;
    int error;

    memset(&param, 0, sizeof(param));
    param.type = ARG_INT;
    param.min = 1;
    param.max = 1000000; // Arbitrary max size
    param.ylabel = "SIZE         -- New size in bytes\n";
    param.hlabel = "SIZE         -- New size in bytes\n";
    param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;

    if ((error = getparameter(argc, argv, u, &param))) {
        Print("[os-demo] Invalid size parameter.\n", 0, 0, 0, 0, 0, 0, 0);
        return error;
    }

    size_t size = param.value.v_int;
    os_demo_region_realloc_handler(size);
    return 0;
}

static int os_demo_std_region_alloc(int argc, char **argv, struct user *u)
{
    struct parameter param;
    int error;

    memset(&param, 0, sizeof(param));
    param.type = ARG_INT;
    param.min = 1;
    param.max = 1000000; // Arbitrary max size
    param.ylabel = "SIZE         -- Size in bytes\n";
    param.hlabel = "SIZE         -- Size in bytes\n";
    param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;

    if ((error = getparameter(argc, argv, u, &param))) {
        Print("[os-demo] Invalid size parameter.\n", 0, 0, 0, 0, 0, 0, 0);
        return error;
    }

    size_t size = param.value.v_int;
    os_demo_std_region_alloc_handler(size);
    return 0;
}

static int os_demo_std_region_free(int argc, char **argv, struct user *u)
{
    os_demo_std_region_free_handler();
    return 0;
}

static int os_demo_std_region_realloc(int argc, char **argv, struct user *u)
{
    struct parameter param;
    int error;

    memset(&param, 0, sizeof(param));
    param.type = ARG_INT;
    param.min = 1;
    param.max = 1000000; // Arbitrary max size
    param.ylabel = "SIZE         -- New size in bytes\n";
    param.hlabel = "SIZE         -- New size in bytes\n";
    param.flag = ARG_MIN | ARG_MAX | ARG_LABEL;

    if ((error = getparameter(argc, argv, u, &param))) {
        Print("[os-demo] Invalid size parameter.\n", 0, 0, 0, 0, 0, 0, 0);
        return error;
    }

    size_t size = param.value.v_int;
    os_demo_std_region_realloc_handler(size);
    return 0;
}

// ######################### Other existing command handlers ###########################

static int second_os_demo_config_test1(int argc, char *argv[], struct user *u)
{
    os_demo_create_socket_task();
    printf("%s,%d\n", __FUNCTION__, __LINE__);
    return 0;
}

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

    char *numStr = param.value.v_string;
    char taskNameBuf[16] = "DM";
    strcat(taskNameBuf, numStr);

    int tid = taskNameToId(taskNameBuf);
    if (tid == ERROR) {
        Print("Error: No task found with name '%s'\n", taskNameBuf, 0, 0, 0, 0, 0, 0);
        return -1;
    }

    if (taskDelete(tid) == OK) {
        Print("Success: Deleted task '%s' (ID = 0x%X)\n", taskNameBuf, tid, 0, 0, 0, 0, 0);
    } else {
        Print("Error: Failed to delete task '%s' (ID = 0x%X)\n", taskNameBuf, tid, 0, 0, 0, 0, 0);
    }

    return 0;
}

static int second_os_demo_config_test3(int argc, char **argv, struct user *u)
{
    printf("%s,%d\n", __FUNCTION__, __LINE__);
    return 0;
}

static int second_os_demo_config_msg_event(int argc, char **argv, struct user *u)
{
    return 0;
}

static int second_os_demo_config_msg(int argc, char **argv, struct user *u)
{
    printf("%s,%d\n", __FUNCTION__, __LINE__);
    return 0;
}

static int second_os_demo_config_event(int argc, char **argv, struct user *u)
{
    printf("%s,%d\n", __FUNCTION__, __LINE__);
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