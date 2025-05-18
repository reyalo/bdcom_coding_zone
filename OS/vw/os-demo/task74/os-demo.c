#include <ip/types.h>
#include <ip/in.h>
#include <ip/inet.h>
#include <ip/socket.h>
#include <libfile/file_sys.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <global.h>
#include <ip/target.h>
#include <libcmd/cmdparse.h>
#include <ip/trace.h>
#include <ip/ip_compat.h>
#include <ip/systm.h>
#include <libsys/memory.h>
#include <libsys/vos/vos_msgq.h>
#include <libsys/vos/vos_task.h>
#include <ip/ioctl.h>
#include <ip/msg.h>
#include <ip/errno.h>
#include <libsys/timer.h>

#define LEN_MAC               6
#define CACHE_IP_START        0xC0A80100 // 192.168.1.0
#define MAX_IP_CACHE_NUM      256
#define MAX_CACHE_NODES       100
#define MAX_AGE               10

#define MSG_QUEUE_SIZE        20
#define TASK_STACK_SIZE       1024
#define TASK_PRIORITY_DMAG    128
#define TASK_PRIORITY_DMMC    129
#define TASK_PRIORITY_DMRC    130
#define DELAY_BASE_MS         100
#define DELAY_RANDOM_MS       500
#define TIMER_INTERVAL_SEC    1
#define MAC_RANDOM_MAX        256
#define SEM_INITIAL_VALUE     1

typedef struct ip_cache_node {
    unsigned int ip;
    unsigned char mac[LEN_MAC];
    unsigned int age;
    struct ip_cache_node *next;
} ip_cache_node_t;

typedef struct {
    ip_cache_node_t *node; // Pointer to the node
} ip_cache_msg_t;

static MSG_Q_ID g_msgQ_id = NULL;
static SEM_ID g_list_sem = NULL;
static ip_cache_node_t *g_ip_cache_head = NULL;
static ULONG g_age_timer_id = INVALID_TIMER_ID;

void os_demo_init(void);
void os_demo_ip_cache_create_process(void);
void os_demo_ip_cache_get_mac_process(void);
void os_demo_ip_cache_age_check_process(void);
void age_timer_callback(ULONG arg);
void os_demo_main_task_init(void);

void os_demo_init(void)
{
    Print("_______________REYAL_______________(11)\n");
    os_demo_main_task_init();
}

void os_demo_main_task_init(void){
    // Create message queue
    g_msgQ_id = msgQCreate(MSG_QUEUE_SIZE, sizeof(ip_cache_msg_t), MSG_Q_FIFO);
    if (!g_msgQ_id) {
        Print("Failed to create message queue\n");
        return;
    }

    // Create semaphore for linked list
    g_list_sem = semBCreate(SEM_Q_FIFO, SEM_INITIAL_VALUE);
    if (!g_list_sem) {
        Print("Failed to create semaphore\n");
        msgQDelete(g_msgQ_id);
        return;
    }

    // Spawn tasks
    if (taskSpawn("DMCR", TASK_PRIORITY_DMRC, 0, TASK_STACK_SIZE, (FUNCPTR)os_demo_ip_cache_create_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
        Print("Failed to spawn DMCR task\n");
        return;
    }
    if (taskSpawn("DMMC", TASK_PRIORITY_DMMC, 0, TASK_STACK_SIZE, (FUNCPTR)os_demo_ip_cache_get_mac_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
        Print("Failed to spawn DMMC task\n");
        return;
    }
    if (taskSpawn("DMAG", TASK_PRIORITY_DMAG, 0, TASK_STACK_SIZE, (FUNCPTR)os_demo_ip_cache_age_check_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
        Print("Failed to spawn DMAG task\n");
        return;
    } 
}

void os_demo_ip_cache_create_process(void)
{
    srand(sysClkRateGet());
    while (1) {
        // Create new node
        ip_cache_node_t *node = (ip_cache_node_t *)malloc(sizeof(ip_cache_node_t));
        if (!node) {
            Print("Failed to allocate node\n");
            taskDelay(1000);
            continue;
        }
        node->ip = CACHE_IP_START + (rand() % MAX_IP_CACHE_NUM);
        node->age = 0;
        memset(node->mac, 0, LEN_MAC);
        node->next = NULL;

        // Add to linked list
        semTake(g_list_sem, WAIT_FOREVER);
        node->next = g_ip_cache_head;
        g_ip_cache_head = node;
        semGive(g_list_sem);

        Print("Created node with IP 0x%X\n", node->ip);

        // Send message to DMMC
        ip_cache_msg_t msg = { .node = node };
        if (msgQSend(g_msgQ_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) != 0) {
            Print("Failed to send message to DMMC\n");
            // we should delete node.
        }

        // Random delay (100–599 ticks, ~100–599ms)
        taskDelay(DELAY_BASE_MS + (rand() % DELAY_RANDOM_MS));
    }
}

void os_demo_ip_cache_get_mac_process(void)
{
    ip_cache_msg_t msg;
    while (1) {
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER) != ERROR) {

            semTake(g_list_sem, WAIT_FOREVER);

            if (msg.node) {
                unsigned int ip_val = htonl(msg.node->ip); // Convert to network byte order
                memcpy(msg.node->mac, &ip_val, 4); // Copy 4 bytes safely

                // Set last 2 bytes to random values
                msg.node->mac[4] = (unsigned char)(rand() % MAC_RANDOM_MAX);
                msg.node->mac[5] = (unsigned char)(rand() % MAC_RANDOM_MAX);

                Print("Set MAC %02X:%02X:%02X:%02X:%02X:%02X for IP 0x%X\n",
                      msg.node->mac[0], msg.node->mac[1], msg.node->mac[2], msg.node->mac[3],
                      msg.node->mac[4], msg.node->mac[5], msg.node->ip);
            } else {
                Print("Received null node pointer\n");
                // should delete the node
            }
            semGive(g_list_sem);
        }
    }
}

void os_demo_ip_cache_age_check_process(void)
{
    // Create and start age timer
    TIMER_USER_DATA timer_data;
    timer_data.cb.fun = age_timer_callback;
    timer_data.cb.arg = 0;
    if (sys_add_timer(TIMER_CALLBACK_METHOD | TIMER_LOOP, &timer_data, &g_age_timer_id) == SYS_NOERR) {
        if (sys_start_timer(g_age_timer_id, TIMER_RESOLUTION_S | TIMER_INTERVAL_SEC) != SYS_NOERR) {
            Print("Failed to start age timer\n");
            sys_delete_timer(g_age_timer_id);
        }
    } else {
        Print("Failed to create age timer\n");
    }
}

void age_timer_callback(ULONG arg)
{
    semTake(g_list_sem, WAIT_FOREVER);
    ip_cache_node_t *node = g_ip_cache_head;
    ip_cache_node_t *prev = NULL;
    while (node) {
        node->age++;

        Print("Node IP: 0x%X, MAC: %02X:%02X:%02X:%02X:%02X:%02X, Age: %d\n",
            node->ip, node->mac[0], node->mac[1], node->mac[2], node->mac[3],
            node->mac[4], node->mac[5], node->age);

        if (node->age >= MAX_AGE) {
            if (prev) {
                prev->next = node->next;
            } else {
                g_ip_cache_head = node->next;
            }
            ip_cache_node_t *temp = node;
            node = node->next;
            free(temp);
            Print("Deleted node with IP 0x%X, age %d\n", temp->ip, temp->age);
        } else {
            prev = node;
            node = node->next;
        }
    }
    Print("Age increment complete\n");
    semGive(g_list_sem);
}
