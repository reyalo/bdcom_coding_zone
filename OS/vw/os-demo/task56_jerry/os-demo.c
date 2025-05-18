#include <msgQLib.h>
#include <eventLib.h>
#include <taskLib.h>
#include <ip/socket.h>
#include <ip/inet.h>
#include <ip/msg.h>
#include <libfile/file_sys.h>


MSG_Q_ID os_demo_msgq_id;
int os_demo_task_id;

#define SOCKET_SERVER_MSG_FILE 1
#define SOCKET_SERVER_MSG_TEXT 2
#define TASK_PRIORITY 128
#define STACK_SIZE 1024 * 8
#define BUFFER_SIZE 1024
#define FILE_NAME_LEN 32
#define SERVER_TEXT_PORT 5001
#define SERVER_FILE_PORT 5002
#define REMOTE_COMMUNICATION_PKG_LEN 64
#define i8	char
#define i16	short
#define	MAX_NUM(a,b) (((a)>(b))?(a):(b))

void os_demo_socket_text(int);
void os_demo_socket_file(int);
void os_demo_socket_message_event_process(int sock_tfd, int sock_ffd);
int os_demo_socket_create(int port, int *fd);
void get_content_from_specify_file(int *file_size, char **ptr_content);
void os_demo_main_process(void);
void os_demo_register_cmds(void);
extern unsigned long Print(char *format, ...);
void os_demo_main_task_init(void);

enum remote_communication_msg_type
{
    REMOTE_COMMUNICATION_MSE_TYPE_QRY = 1,    /* query */
    REMOTE_COMMUNICATION_MSE_TYPE_QRY_ACK,    /* Response query */
    REMOTE_COMMUNICATION_MSE_TYPE_CFM,        /* confirm */
	REMOTE_COMMUNICATION_MSE_TYPE_CFM_ACK	  /* Response confirm */
};

#pragma pack(1)
struct remote_communication_msg
{
    i8 msg_type;                             /* message type from remote_communication_msg_type */
    i16 msg_len;                             /* The length of message */
    i8 msg[0];    							 /* content of message */
};
#pragma pack()


struct demo_msg
{
	unsigned long msg_type;
	unsigned long data;
	unsigned long reserved1;
	unsigned long sub_type;
};

enum msg_type
{
	DEMO_MSG_TYPE1 = 1,
	DEMO_MSG_TYPE2,
	DEMO_MSG_TYPE3,
	DEMO_MSG_TYPE_EVENT
};

#define DEMO_EVENT_A	0x00000001 
#define DEMO_EVENT_B    0x00000002
#define DEMO_EVENT_C    0x00000004
#define DEMO_EVENT_MSG  0x00000008
#define DEMO_EVENT_MASK 0x0000000F

#define DEMO_MSG_LEN 20

extern MSG_Q_ID os_demo_msgq_id;
extern int os_demo_task_id;

extern unsigned long Print(char *format, ...);

void os_demo_init(void)
{
	os_demo_register_cmds();
	
	os_demo_main_task_init();
	
	return;
}

void os_demo_main_task_init(void)
{
	int sock_tfd = -1, sock_ffd = -1;
	
	os_demo_msgq_id = msgQCreate(DEMO_MSG_LEN, sizeof(struct demo_msg), MSG_Q_FIFO);
	if (os_demo_msgq_id == NULL)
	{
		Print("Failed to create message queue\n");
		return;
	}
	
	if (os_demo_socket_create(SERVER_TEXT_PORT, &sock_tfd) != 0)
	{
		Print("Socket for text Init failed.\n");
		goto resource_release;
	}
	
	if (os_demo_socket_create(SERVER_FILE_PORT, &sock_ffd) != 0)
	{
		Print("Socket for file Init failed.\n");
		goto resource_release;
	}
	
	if (socket_register(sock_tfd, (ULONG)os_demo_msgq_id, SOCKET_SERVER_MSG_TEXT))
	{
		Print("Socket sock_tfd register to msgq failed.\n");
		goto resource_release;
	}

	if (socket_register(sock_ffd, (ULONG)os_demo_msgq_id, SOCKET_SERVER_MSG_FILE))
	{
		Print("Socket sock_ffd register to msgq failed.\n");
		goto resource_release;
	}
	
	os_demo_task_id = taskSpawn("SKEM", TASK_PRIORITY, 0, STACK_SIZE, 
		(FUNCPTR)os_demo_socket_message_event_process, sock_tfd, sock_ffd, 0, 0, 0, 0, 0, 0, 0, 0);
	if (os_demo_task_id == ERROR)
	{
		Print("Failed to create task SKTF\n");
		goto resource_release;
	}

    return;
	
resource_release:
	if(sock_tfd != -1)
		so_close(sock_tfd);
	
	if(sock_ffd != -1)
		so_close(sock_ffd);
	
	return;
}

void os_demo_socket_message_event_process(int sock_tfd, int sock_ffd)
{
	int rv;
	UINT32 events;
	struct demo_msg msg;
	
	while(1)
	{
		rv = msgQReceive(os_demo_msgq_id, (char *)&msg, sizeof(msg), WAIT_FOREVER);
		if (rv == ERROR)
		{
			Print("Failed to receive message\n");
			continue;
		}
		
		switch(msg.msg_type)
		{
			case DEMO_MSG_TYPE1:
			case DEMO_MSG_TYPE2:
			case DEMO_MSG_TYPE3:
				Print("Task %s, Received a normal message 0x%x\n", taskName(taskIdSelf()), msg.msg_type);
				break;
			case SOCKET_DATARCVD:
				switch(msg.sub_type)
				{
					case SOCKET_SERVER_MSG_TEXT:
						os_demo_socket_text(sock_tfd);
						break;
					case SOCKET_SERVER_MSG_FILE:
						os_demo_socket_file(sock_ffd);
						break;
					default:
						Print("Task %s, An undefined message %x was received.\n", taskName(taskIdSelf()), msg.msg_type);
						break;
						
				}
				break;
			case DEMO_MSG_TYPE_EVENT:
				events = 0;
				rv = eventReceive(DEMO_EVENT_MASK, EVENTS_WAIT_ANY, NO_WAIT, &events);
				if (rv == ERROR)
				{
					continue;
				}
				
				if (events & DEMO_EVENT_A)
				{
					Print("Task %s, Received event 0x%x\n", taskName(taskIdSelf()), DEMO_EVENT_A);
				}
				
				if (events & DEMO_EVENT_B)
				{
					Print("Task %s, Received event 0x%x\n", taskName(taskIdSelf()), DEMO_EVENT_B);
				}
				
				if (events & DEMO_EVENT_C)
				{
					Print("Task %s, Received event 0x%x\n", taskName(taskIdSelf()), DEMO_EVENT_C);
				}
				break;
			default:
				Print("Task %s, An undefined message %x was received.\n", taskName(taskIdSelf()), msg.msg_type);
				break;
		}
	}
	
	so_close(sock_tfd);
	so_close(sock_ffd);
	
	return;
}

int os_demo_socket_create(int port, int *fd)
{
	int sockfd;
	struct soaddr server;
	struct soaddr_in *servaddr;
	
	if ((sockfd = so_socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		return -1;
	}
	
	servaddr = (struct soaddr_in *)&server;
	servaddr->sin_len = sizeof(struct soaddr_in);
    servaddr->sin_family    = AF_INET;
    servaddr->sin_port      = htons(port);            
    servaddr->sin_addr.s_addr = INADDR_ANY;
           
    if (so_bind(sockfd, &server, sizeof(server)) < 0) 
	{
		so_close(sockfd);
		return -1;
	}
	
	*fd = sockfd;
	return 0;
}

void os_demo_socket_text(int sockfd)
{
	int addr_len, recv_bytes;
	static int is_recv_query = 0;
	struct soaddr client;   
	struct soaddr_in *cliaddr;
	char send_buf[REMOTE_COMMUNICATION_PKG_LEN], recv_buf[REMOTE_COMMUNICATION_PKG_LEN];
	struct remote_communication_msg *send_prcm, *recv_prcm;
	
    //receive message from server
    addr_len = 0;
	cliaddr = (struct soaddr_in *)&client;
	send_prcm = (struct remote_communication_msg *)send_buf;
	recv_prcm = (struct remote_communication_msg *)recv_buf;
	
	memset(&recv_buf, 0, REMOTE_COMMUNICATION_PKG_LEN);
	if ((recv_bytes = so_recvfrom(sockfd, recv_buf, REMOTE_COMMUNICATION_PKG_LEN, MSG_DONTWAIT, &client, &addr_len)) < 0)
	{
		Print("The length of received data is less than 0\n");
		return;
	}
	
	Print("%s\n", recv_prcm->msg);
	
	switch (recv_prcm->msg_type)
	{
		case REMOTE_COMMUNICATION_MSE_TYPE_QRY:
			is_recv_query = 1;
			send_prcm->msg_type = REMOTE_COMMUNICATION_MSE_TYPE_QRY_ACK;
			sprintf(send_prcm->msg, "Hi %s %u", inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port));
			send_prcm->msg_len = strlen(send_prcm->msg);
			break;
		case REMOTE_COMMUNICATION_MSE_TYPE_CFM:
			if (is_recv_query == 0)
			{
				Print("Didn't recive query from %s %u\n", inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port));
				break;
			}
			send_prcm->msg_type = REMOTE_COMMUNICATION_MSE_TYPE_CFM_ACK;
			sprintf(send_prcm->msg, "How do yo do %s %u", inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port));
			send_prcm->msg_len = strlen(send_prcm->msg);
			is_recv_query = 0;
			break;
		default:
			break;
	}
	
	if (so_sendto(sockfd, send_prcm, sizeof(struct remote_communication_msg) + send_prcm->msg_len, 0, &client, sizeof(client)) < 0)
	{
		Print("Data send failed.\n");
		return;
	}
	
	return;
}

void get_content_from_specify_file(int *file_size, char **ptr_content)
{
	char file_name[FILE_NAME_LEN] = {0};
	
	strncpy(file_name, *ptr_content, FILE_NAME_LEN);
	
	{
		FCB_POINT * fp;
		
		if ((fp = file_open(file_name, "r", NULL)) == NULL) 
		{
			sprintf(*ptr_content, "filename %s", file_name);
			*file_size = strlen(*ptr_content);
		
			return;
		}
		
		{
			int f_len = getfilelen(fp);
			if(f_len > BUFFER_SIZE)
				*ptr_content = malloc(f_len + 1);
			
			file_read(fp, *ptr_content, f_len);
			*file_size = f_len;
			
			Print("File size: %d bytes\n", *file_size);
		}
		
		file_close(fp);
	}
	
	return;
}

void os_demo_socket_file(int sockfd)
{
    int file_size, addrlen;
	char buffer[BUFFER_SIZE];
	char *ptr_content;
	struct soaddr client;
	
	memset(&client, 0, sizeof(client));
	
	// recv
	{
		memset(buffer, 0, BUFFER_SIZE);
	
		if (so_recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_DONTWAIT, &client, &addrlen) < 0)
		{
			Print("Receiving socket failed.\n");
		}
	}
	
	{
		ptr_content = buffer;
		get_content_from_specify_file(&file_size, &ptr_content);
	}
	
	// send
	{
		int left_need_to_send = file_size, already_send = 0;
		
		while (left_need_to_send)
		{
			if ((already_send += so_sendto(sockfd, 
				ptr_content + already_send, 
				left_need_to_send > BUFFER_SIZE?BUFFER_SIZE:left_need_to_send, 0, &client, sizeof(client))) < 0)
			{
				Print("Sending data failed.\n");
				break;
			}
			
			left_need_to_send -= already_send;
		}
	}
	
	if (file_size >= BUFFER_SIZE)
		free(ptr_content);

	return;
}



