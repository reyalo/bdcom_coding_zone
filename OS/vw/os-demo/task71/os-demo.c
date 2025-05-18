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

#define LEN_MAC 6
#define CACHE_IP_START 0xC0A80100 // 192.168.1.0
#define MAX_IP_CACHE_NUM 256
#define MAX_CACHE_NODES 100
#define MAX_AGE         10

typedef struct ip_cache_node {
    unsigned int ip;
    unsigned char mac[LEN_MAC];
    unsigned int age;
    struct ip_cache_node *next;
} ip_cache_node_t;

typedef struct {
    unsigned int ip;
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

void os_demo_init(void)
{
    Print("_______________IP Cache Demo_______________\n");

    // Create message queue
    g_msgQ_id = msgQCreate(20, sizeof(ip_cache_msg_t), MSG_Q_FIFO);
    if (!g_msgQ_id) {
        Print("Failed to create message queue\n");
        return;
    }

    // Create semaphore for linked list
    g_list_sem = semBCreate(SEM_Q_FIFO, 1);
    if (!g_list_sem) {
        Print("Failed to create semaphore\n");
        msgQDelete(g_msgQ_id);
        return;
    }

    // Spawn tasks
    if (taskSpawn("DMCR", 130, 0, 1024, (FUNCPTR)os_demo_ip_cache_create_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
        Print("Failed to spawn DMCR task\n");
        return;
    }
    if (taskSpawn("DMMC", 129, 0, 1024, (FUNCPTR)os_demo_ip_cache_get_mac_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
        Print("Failed to spawn DMMC task\n");
        return;
    }
    if (taskSpawn("DMAG", 128, 0, 1024, (FUNCPTR)os_demo_ip_cache_age_check_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
        Print("Failed to spawn DMAG task\n");
        return;
    }

    // Create and start age timer
    TIMER_USER_DATA timer_data;
    timer_data.cb.fun = age_timer_callback;
    timer_data.cb.arg = 0;
    if (sys_add_timer(TIMER_CALLBACK_METHOD | TIMER_LOOP, &timer_data, &g_age_timer_id) == SYS_NOERR) {
        if (sys_start_timer(g_age_timer_id, TIMER_RESOLUTION_S | 1) != SYS_NOERR) {
            Print("Failed to start age timer\n");
            sys_delete_timer(g_age_timer_id);
        }
    } else {
        Print("Failed to create age timer\n");
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
        ip_cache_msg_t msg = { .ip = node->ip };
        if (msgQSend(g_msgQ_id, (char *)&msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) != 0) {
            Print("Failed to send message to DMMC\n");
        }

        // Random delay (100–599 ticks, ~100–599ms)
        taskDelay(100 + (rand() % 500));
    }
}

void os_demo_ip_cache_get_mac_process(void)
{
    ip_cache_msg_t msg;
    while (1) {
        if (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER) != ERROR) {
            // Find node with matching IP
            semTake(g_list_sem, WAIT_FOREVER);
            ip_cache_node_t *node = g_ip_cache_head;
            while (node && node->ip != msg.ip) {
                node = node->next;
            }
            if (node) {
                Print("Found node with IP 0x%X, node at 0x%X\n", node->ip, (unsigned int)node);
                // Copy first 4 bytes of IP to MAC using memcpy
                unsigned int ip_val = htonl(node->ip); // Convert to network byte order
                memcpy(node->mac, &ip_val, 4); // Copy 4 bytes safely
                // Set last 2 bytes to random values
                node->mac[4] = (unsigned char)(rand() % 256);
                node->mac[5] = (unsigned char)(rand() % 256);
                Print("Set MAC %02X:%02X:%02X:%02X:%02X:%02X for IP 0x%X\n",
                      node->mac[0], node->mac[1], node->mac[2], node->mac[3],
                      node->mac[4], node->mac[5], node->ip);
            } else {
                Print("Node with IP 0x%X not found\n", msg.ip);
            }
            semGive(g_list_sem);
        }
    }
}

void os_demo_ip_cache_age_check_process(void)
{
    while (1) {
        semTake(g_list_sem, WAIT_FOREVER);
        ip_cache_node_t *node = g_ip_cache_head;
        ip_cache_node_t *prev = NULL;
        while (node) {
            node->age++;
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
        semGive(g_list_sem);
        taskDelay(1000); // Wait for next check (adjustable)
    }
}

void age_timer_callback(ULONG arg)
{
    semTake(g_list_sem, WAIT_FOREVER);
    ip_cache_node_t *node = g_ip_cache_head;
    while (node) {
        node->age++;
        if (node->age >= MAX_AGE) {
            ip_cache_node_t *temp = node;
            node = node->next;
            if (temp == g_ip_cache_head) {
                g_ip_cache_head = temp->next;
            } else {
                ip_cache_node_t *prev = g_ip_cache_head;
                while (prev->next != temp) prev = prev->next;
                prev->next = temp->next;
            }
            free(temp);
            Print("Deleted node with IP 0x%X, age %d\n", temp->ip, temp->age);
        } else {
            node = node->next;
        }
    }
    semGive(g_list_sem);
}