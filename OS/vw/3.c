#include <vxWorks.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <eventLib.h>
#include <string.h>
#include <stdio.h>

#define MAX_MSG_NUM     20
#define MAX_MSG_LEN     100
#define MAX_DATA_LEN    100
#define EVENT_MSG_READY 0x01
#define ALL_EVENTS      0xFFFFFFFF

MSG_Q_ID g_msgQ_id;
TASK_ID g_dmne_tid;

/* Message structure */
typedef struct {
    int type;
    char data[MAX_DATA_LEN];
} message_t;

/* Combined Message/Event Task using automatic msgQ event */
void os_demo_msg_event_recv(void) {
    Print("DMNE task running. Waiting for events (including message arrival)...\n");

    while (1) {
        UINT32 receivedEvents;

        // Block until an event occurs
        if (eventReceive(ALL_EVENTS, EVENTS_WAIT_OR | EVENTS_WAIT_CLR, WAIT_FOREVER, &receivedEvents) == OK) {
            if (receivedEvents & EVENT_MSG_READY) {
                message_t msg;
                while (msgQReceive(g_msgQ_id, (char *)&msg, sizeof(msg), NO_WAIT) != ERROR) {
                    Print("[DMNE] Msg received: type=%d, data=%s\n", msg.type, msg.data);
                }
            }

            // Handle other user-defined events
            UINT32 otherEvents = receivedEvents & ~EVENT_MSG_READY;
            if (otherEvents) {
                Print("[DMNE] Events received: 0x%X\n", otherEvents);
            }
        }
    }
}

/* CLI Trigger Function to Create Queue & Task */
void os_demo_create_dmne_task(void) {
    if (!g_msgQ_id) {
        g_msgQ_id = msgQCreate(MAX_MSG_NUM, MAX_MSG_LEN, MSG_Q_FIFO);
        if (!g_msgQ_id) {
            Print("Failed to create message queue\n");
            return;
        }

        // Automatically generate event when a message is queued
        if (msgQEvStart(g_msgQ_id, EVENT_MSG_READY) != OK) {
            Print("Failed to link message queue to event\n");
            return;
        }
    }

    g_dmne_tid = taskSpawn("DMNE", 128, 0, 2048, (FUNCPTR)os_demo_msg_event_recv, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (g_dmne_tid == (TASK_ID)-1) {
        Print("Failed to spawn DMNE task\n");
        return;
    }

    Print("DMNE Task Created: 0x%X\n", (int)g_dmne_tid);
}

/* External API to send a message */
void os_demo_send_message(int type, const char *data) {
    message_t msg;
    msg.type = type;
    strncpy(msg.data, data, MAX_DATA_LEN - 1);
    msg.data[MAX_DATA_LEN - 1] = '\0';

    msgQSend(g_msgQ_id, (char *)&msg, sizeof(msg), WAIT_FOREVER, MSG_PRI_NORMAL);
    // No need to send event manually when msgQEvStart is used
}

/* External API to send a custom event */
void os_demo_send_event(UINT32 bitmask) {
    eventSend(g_dmne_tid, bitmask);
}
