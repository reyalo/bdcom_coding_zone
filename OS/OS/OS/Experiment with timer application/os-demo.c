#include "list.h"
#include "os-demo.h"
#include <taskLib.h>
#include <ip/socket.h>
#include <ip/inet.h>
#include <ip/msg.h>
#include <libfile/file_sys.h>

#define AGEING_TIME	100
#define LEN_MAC 6
#define LEN_MSGQ 30
#define PRIORITY 128
#define STACK_SIZE 1024
#define CACHE_IP_START 0xc0a80001
#define MAX_IP_CACHE_NUM 8000
#define DEMO_MSG_TYPE_TIMER 1
#define CACHE_LIFETIME 1*1000 /* in unit of millisecond */
#define AGEOUT_INTERVAL 500 /* in unit of millisecond */

#define MAC_ADDR_CPY(dst,src)	 { *(uint16 *)(dst) = *(uint16 *)(src);\
	*(uint16*)( ((uint8*)dst)+ 2 ) = *(uint16*)( ((uint8*)src) + 2 );\
	*(uint16*)( ((uint8*)dst)+ 4 ) = *(uint16*)( ((uint8*)src) + 4 );}

struct ip_cache_node
{
	unsigned int ip;
	unsigned char mac[LEN_MAC];
	unsigned int age;
	struct list_head list;
};

struct ip_cache_ip_msg
{
	unsigned int ip;
	unsigned char mac[LEN_MAC];
};

struct ip_cache_msg
{
	unsigned long msg_type;
	unsigned long reserved1;
	unsigned long reserved2;
	unsigned long reserved3;
};

SEM_ID ip_cache_sem_id;
MSG_Q_ID ip_cache_msgq_id;
MSG_Q_ID ip_cache_msgq_query_ip_id;
MSG_Q_ID ip_cache_msgq_reply_ip_id;
struct list_head node_list;

void os_demo_main_task_init(void);
void os_demo_register_cmds(void);
void os_demo_ip_cache_create_process(void);
void os_demo_ip_cache_age_check_process(void);
void os_demo_ip_cache_get_mac_process(void);

void os_demo_init(void)
{
	os_demo_register_cmds();
	
	os_demo_main_task_init();
	
	return;
}

void os_demo_main_task_init(void)
{
	unsigned long timer_id;
	TIMER_USER_DATA timer_ud;
	
	if ((ip_cache_sem_id = semBCreate(SEM_Q_FIFO, SEM_FULL)) == NULL)
	{
		Print("Failed to create the binary semaphore, error %d\n", errno);
		return;
	}
	
	if ((ip_cache_msgq_id = msgQCreate(LEN_MSGQ, sizeof(struct ip_cache_msg), MSG_Q_FIFO)) == NULL)
	{
		Print("Failed to create message queue, errors %d\n", errno);
		return;
	}
	
	if ((ip_cache_msgq_query_ip_id = msgQCreate(LEN_MSGQ, sizeof(struct ip_cache_ip_msg), MSG_Q_FIFO)) == NULL)
	{
		Print("Failed to create message queue, errors %d\n", errno);
		return;
	}
	
	if ((ip_cache_msgq_reply_ip_id = msgQCreate(LEN_MSGQ, sizeof(struct ip_cache_ip_msg), MSG_Q_FIFO)) == NULL)
	{
		Print("Failed to create message queue, errors %d\n", errno);
		return;
	}
	
	timer_ud.msg.qid = ip_cache_msgq_id;
	timer_ud.msg.msg_buf[0] = DEMO_MSG_TYPE_TIMER; /* buf[0] acts as the message type */
	sys_add_timer(TIMER_LOOP | TIMER_MSG_METHOD, &timer_ud, &timer_id);
	sys_start_timer(timer_id, TIMER_RESOLUTION_S | 10);
	
	if ((taskSpawn("DMCR", PRIORITY, 0, STACK_SIZE, (FUNCPTR)os_demo_ip_cache_create_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR)
		Print("Failed to create the task Create\n");

	if ((taskSpawn("DMAG", PRIORITY, 0, STACK_SIZE, (FUNCPTR)os_demo_ip_cache_age_check_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR)
		Print("Failed to create the task Create\n");
	
	if ((taskSpawn("DMMC", PRIORITY, 0, STACK_SIZE, (FUNCPTR)os_demo_ip_cache_get_mac_process, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR)
		Print("Failed to create the task Create\n");
	
	return;
}

void os_demo_ip_cache_create_process(void)
{
	struct ip_cache_ip_msg ip_msg;
	
	INIT_LIST_HEAD(&node_list);	
	
	while(1)
	{
		unsigned int ip = CACHE_IP_START + rand() % MAX_IP_CACHE_NUM;
		struct ip_cache_node *new_cache = NULL;
		
		taskDelay(100 + rand() % 500); /* unit tick */
		
		memset(&ip_msg, 0, sizeof(ip_msg));
		ip_msg.ip = ip;
		if ((msgQSend(ip_cache_msgq_query_ip_id, (char *)&ip_msg, sizeof(ip_msg), NO_WAIT, MSG_PRI_NORMAL)) != ERROR)
		{
			memset(&ip_msg, 0, sizeof(ip_msg));
			if (msgQReceive(ip_cache_msgq_reply_ip_id, (char *)&ip_msg, sizeof(ip_msg), WAIT_FOREVER) == ERROR)
				Print("Failed to receive message\n");
		}

		if ((new_cache = (struct ip_cache_node *) malloc(sizeof(struct ip_cache_node))) == NULL)
		{
			Print("Failed to get new memory\n");
			continue;
		}
		
		memset(new_cache, 0, sizeof(struct ip_cache_node));
		new_cache->ip = ip;
		MAC_ADDR_CPY(new_cache->mac, ip_msg.mac);
		
		semTake(ip_cache_sem_id, WAIT_FOREVER);
		list_add(&new_cache->list, &node_list);
		semGive(ip_cache_sem_id);
		
#ifdef ENABLE_DEBUG
		os_demo_ip_cache_create_counter++;
		Print("Creating a ip cache Which ip is %x\n", ip);
#endif
	}

	return;
}

void os_demo_ip_cache_age_check_process(void)
{
	int rv;
	struct ip_cache_msg msg;
	
	while(1)
	{
		rv = msgQReceive(ip_cache_msgq_id, (char *)&msg, sizeof(msg), WAIT_FOREVER);
		if (rv == ERROR)
		{
			Print("Failed to receive message\n");
			continue;
		}
		
		if (msg.msg_type == DEMO_MSG_TYPE_TIMER)
		{
			struct list_head *pos, *temp;
			
			semTake(ip_cache_sem_id, WAIT_FOREVER);
			list_for_each_safe(pos, temp, &node_list) 
			{
				struct ip_cache_node *ip_cache = list_entry(pos, struct ip_cache_node, list);
				
				ip_cache->age++;					/* Add 1 every time */
				if (ip_cache->age >= AGEING_TIME)	/* Delete the node when age reach to 100 */
				{
					list_del(pos);
					free(ip_cache);
				}
				
				taskDelay(1);
				
#ifdef ENABLE_DEBUG
				os_demo_ip_cache_delete_counter++;
				Print("Deleting a ip cache Which ip is %x\n", ip_cache->ip);
#endif
			}
			semGive(ip_cache_sem_id);
		}
	}

	return;
}

void os_demo_ip_cache_get_mac_process(void)
{
	short rand_num;
	struct ip_cache_ip_msg ip_msg;
	
	while(1)
	{
		rand_num = rand();
		if ((msgQReceive(ip_cache_msgq_query_ip_id, (char *)&ip_msg, sizeof(ip_msg), WAIT_FOREVER)) == ERROR)
		{
			Print("Failed to receive message\n");
			continue;
		}
		
		memcpy(&ip_msg.mac, &ip_msg.ip, sizeof(ip_msg.ip));
		memcpy(&ip_msg.mac + sizeof(ip_msg.ip), &rand_num, LEN_MAC - sizeof(ip_msg.ip));
		
#ifdef ENABLE_DEBUG
		Print("%x,%s\n", ip_msg.ip, ether2str(ip_msg.mac));
#endif

		if ((msgQSend(ip_cache_msgq_reply_ip_id, (char *)&ip_msg, sizeof(ip_msg), NO_WAIT, MSG_PRI_NORMAL)) == ERROR)
			Print("Failed to send message\n");
		
	}
	
	return;
}
