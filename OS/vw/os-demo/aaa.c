#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Replace with actual OS headers
#include "os_task.h"
#include "os_message.h"
#include "os_event.h"

#define DMSG_TASK_NAME "DMSG"
#define DMSG_STACK_SIZE 1024
#define DMSG_PRIORITY   128
#define DMSG_QUEUE_SIZE 10

#define DEVT_TASK_NAME "DEVT"

// Types
typedef struct {
    int type;
    int data;
} message_t;

// Globals
static os_queue_t dmsg_queue = NULL;
static os_task_t dmsg_task_id = NULL;

// Check if a task exists by name
int task_exists(const char *name) {
    os_task_t tid = os_get_task_by_name(name);
    return (tid != NULL);
}

// DMSG Task
void dmsg_task_entry(void *arg) {
    message_t msg;
    while (1) {
        if (os_queue_receive(dmsg_queue, &msg, OS_WAIT_FOREVER) == 0) {
            printf("Task DMSG, Received a message: type %d, data %d\n", msg.type, msg.data);
        }
    }
}

// Main command handler: create DMSG if needed and send message
void os_demo_create_msg_event_tasks(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: os-demo msg <type> <data>\n");
        return;
    }

    int type = atoi(argv[1]);
    int data = atoi(argv[2]);

    // Create DMSG task if not exists
    if (!task_exists(DMSG_TASK_NAME)) {
        dmsg_queue = os_queue_create(DMSG_QUEUE_SIZE, sizeof(message_t));
        if (!dmsg_queue) {
            printf("Failed to create DMSG message queue\n");
            return;
        }

        dmsg_task_id = os_task_create(DMSG_TASK_NAME, dmsg_task_entry, NULL,
                                      DMSG_STACK_SIZE, DMSG_PRIORITY);
        if (!dmsg_task_id) {
            printf("Failed to create DMSG task\n");
            os_queue_delete(dmsg_queue);
            dmsg_queue = NULL;
            return;
        }

        printf("Task %s created\n", DMSG_TASK_NAME);
    }

    os_demo_send_msg(type, data);
}

// Standalone: send a message to DMSG
void os_demo_send_msg(int type, int data) {
    message_t msg = { .type = type, .data = data };

    if (os_queue_send(dmsg_queue, &msg) == 0) {
        printf("Task tty0, sent a message: type %d, data %d\n", type, data);
    } else {
        printf("Failed to send message to %s\n", DMSG_TASK_NAME);
    }
}

// Standalone: send an event to DEVT
void os_demo_send_event(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: os-demo event <event_val>\n");
        return;
    }

    uint32_t event = (uint32_t)strtoul(argv[1], NULL, 0);
    os_task_t devt_id = os_get_task_by_name(DEVT_TASK_NAME);

    if (devt_id) {
        os_send_event(devt_id, event);
        printf("Task tty0, sent an event 0x%x to the receiving task\n", event);
    } else {
        printf("Task %s not found\n", DEVT_TASK_NAME);
    }
}
