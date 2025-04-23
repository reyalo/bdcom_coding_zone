#include <vxWorks.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <eventLib.h>
#include <string.h>
#include <stdio.h>
#include <sysLib.h> 
#include <signal.h>
#include <semLib.h>





int Print(const char *fmt, ...);

typedef struct {
    int type;
    char data[100];
} message_t;

#define DMSG_Q_LEN       20
#define DMSG_MSG_SIZE    sizeof(message_t)
#define DMSG_Q_OPTIONS   MSG_Q_FIFO
#define MAX_DATA_LEN 100

#define EVENT_MSG_READY  0x80000000
#define EVENT_CUSTOM     0x02
static MSG_Q_ID g_msgQ_id = NULL;



