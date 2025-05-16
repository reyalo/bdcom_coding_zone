#include <taskLib.h>
#include <msgQLib.h>
#include <semLib.h>

/* message type(s) */
#define DEMO_MSG_TYPE_TEST 1

#define TEST_TABLE_LEN (16*1024)
#define NUM_WORKERS 4  /* Number of worker tasks in the pool */

/* Message struct compatible with Baud switch software */
typedef struct  
{
    unsigned long msg_type;
    unsigned long count;
    unsigned long reserved1;
    unsigned long reserved2;
} demo_msg_t;

extern unsigned long Print(char *format, ...);
void os_demo_send();
void os_demo_recv();
void worker_task(long arg);

MSG_Q_ID msgq_id;          /* Main receive queue */
MSG_Q_ID worker_q_id;      /* Queue for worker tasks */
SEM_ID worker_sem;         /* Semaphore to signal work availability */
long test_table[TEST_TABLE_LEN];

void os_demo_init()
{
    int task_id, i;

    /* Create main receive queue */
    msgq_id = msgQCreate(256, sizeof(demo_msg_t), MSG_Q_FIFO);
    if (msgq_id == NULL)
    {
        Print("Failed to create main message queue\n");
        return;
    }

    /* Create worker queue */
    worker_q_id = msgQCreate(256, sizeof(demo_msg_t), MSG_Q_FIFO);
    if (worker_q_id == NULL)
    {
        Print("Failed to create worker queue\n");
        msgQDelete(msgq_id);
        return;
    }

    /* Create semaphore for worker synchronization */
    worker_sem = semBCreate(SEM_Q_FIFO, 0);
    if (worker_sem == NULL)
    {
        Print("Failed to create worker semaphore\n");
        msgQDelete(msgq_id);
        msgQDelete(worker_q_id);
        return;
    }

    /* Spawn receiver task */
    task_id = taskSpawn("DMRV", 128, 0, 1024, (FUNCPTR)os_demo_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (task_id == ERROR)
        Print("Failed to create receiving task\n");

    /* Spawn sender task */
    task_id = taskSpawn("DMSD", 128, 0, 1024, (FUNCPTR)os_demo_send, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (task_id == ERROR)
        Print("Failed to create sending task\n");

    /* Spawn worker tasks */
    for (i = 0; i < NUM_WORKERS; i++)
    {
        task_id = taskSpawn("WORK", 129, 0, 1024, (FUNCPTR)worker_task, i, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        if (task_id == ERROR)
            Print("Failed to create worker task %d\n", i);
    }

    return;
}

void os_demo_send()
{
    static unsigned int count = 0;
    demo_msg_t msg = {.msg_type = DEMO_MSG_TYPE_TEST};
    int rv;

    while(1)
    {
        msg.count = ++count;
        rv = msgQSend(msgq_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL);
        if (rv == ERROR)
            Print("Failed to send a message\n");
        else
            Print("Sent a message: type %u, count %u\n", msg.msg_type, msg.count);

        taskDelay(10*100);
    }

    return;
}

void os_demo_recv()
{
    demo_msg_t msg;
    int rv;

    while(1)
    {
        rv = msgQReceive(msgq_id, (char *)&msg, sizeof(msg), WAIT_FOREVER);
        if (rv == ERROR)
        {
            Print("Failed to receive message\n");
            continue;
        }

        switch(msg.msg_type)
        {
            case DEMO_MSG_TYPE_TEST:
                Print("Received a message: type %u, count %u, forwarding to workers\n", DEMO_MSG_TYPE_TEST, msg.count);
                rv = msgQSend(worker_q_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL);
                if (rv == ERROR)
                    Print("Failed to forward message to worker queue\n");
                else
                    semGive(worker_sem); /* Signal a worker to process */
                break;
            default:
                break;
        }
    }

    return;
}

void worker_task(long arg)
{
    demo_msg_t msg;
    int rv;
    long i, j, chunk_start, chunk_end;
    char task_name[10];

    sprintf(task_name, "WORK%d", (int)arg);
    taskNameSet(taskIdSelf(), task_name);
    Print("Worker %s started\n", task_name);

    while(1)
    {
        /* Wait for work */
        semTake(worker_sem, WAIT_FOREVER);

        /* Receive message from worker queue */
        rv = msgQReceive(worker_q_id, (char *)&msg, sizeof(msg), WAIT_FOREVER);
        if (rv == ERROR)
        {
            Print("Worker %s failed to receive message\n", task_name);
            continue;
        }

        if (msg.msg_type == DEMO_MSG_TYPE_TEST)
        {
            Print("Worker %s processing message: type %u, count %u\n", task_name, DEMO_MSG_TYPE_TEST, msg.count);

            /* Process test_table in chunks */
            for (chunk_start = 0; chunk_start < TEST_TABLE_LEN; chunk_start += 1024)
            {
                chunk_end = chunk_start + 1024;
                if (chunk_end > TEST_TABLE_LEN) chunk_end = TEST_TABLE_LEN;

                for (i = chunk_start; i < chunk_end; i++)
                {
                    for (j = 0; j < (1 << 20); j++)
                        test_table[i] = i;
                }

                Print("Worker %s processed chunk %ld to %ld\n", task_name, chunk_start, chunk_end - 1);
                taskDelay(1); /* Yield CPU to other tasks */
            }

            Print("Worker %s work done\n", task_name);
        }
    }

    return;
}