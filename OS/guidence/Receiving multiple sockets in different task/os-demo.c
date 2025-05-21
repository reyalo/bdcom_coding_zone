#include <taskLib.h>
#include <ip/socket.h>
#include <ip/inet.h>
#include <libfile/file_sys.h>

#define TASK_PRIORITY 128
#define STACK_SIZE 1024 * 3
void os_demo_main_process(void);
void os_demo_register_cmds(void);
extern unsigned long Print(char *format, ...);

#define BUFFER_SIZE 1024
#define FILE_NAME_LEN 32
#define SERVER_TEXT_PORT 5001
#define SERVER_FILE_PORT 5002

#define TASK_SOCKET_TEXT "SCKT"
#define TASK_SOCKET_FILE "SCKF"

#define REMOTE_COMMUNICATION_PKG_LEN 64
#define i8	char
#define i16	short

void os_demo_socket_text(void);
void os_demo_socket_file(void);
void get_content_from_specify_file(int *file_size, char **ptr_content);

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

void os_demo_main_task_init(void);

void os_demo_init(void)
{
	os_demo_register_cmds();
	
	os_demo_main_task_init();
	
	return;
}

void os_demo_main_task_init(void)
{
	int task_id;
	
	task_id = taskSpawn(TASK_SOCKET_TEXT, TASK_PRIORITY, 0, STACK_SIZE, (FUNCPTR)os_demo_socket_text, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if(task_id == ERROR)
		Print("Failed to create task %s\n", TASK_SOCKET_TEXT);
	
	task_id = taskSpawn(TASK_SOCKET_FILE, TASK_PRIORITY, 0, STACK_SIZE, (FUNCPTR)os_demo_socket_file, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if(task_id == ERROR)
		Print("Failed to create task %s\n", TASK_SOCKET_FILE);
	
	return;
}

void os_demo_socket_text(void)
{
	int sock_fd = -1, addr_len, is_recv_query = 0;
	struct soaddr server, client;   
	struct soaddr_in *serv_addr, *cli_addr;
	char send_buf[REMOTE_COMMUNICATION_PKG_LEN], recv_buf[REMOTE_COMMUNICATION_PKG_LEN];
	struct remote_communication_msg *send_prcm, *recv_prcm;
	
	if((sock_fd = so_socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		Print("Socket create failed.\n");
		return;
	}
	
	serv_addr = (struct soaddr_in *)&server;
	serv_addr->sin_len = sizeof(struct soaddr_in);
    serv_addr->sin_family    = AF_INET;
    serv_addr->sin_port      = htons(SERVER_TEXT_PORT);            
    serv_addr->sin_addr.s_addr = INADDR_ANY;
           
    if(so_bind(sock_fd, &server, sizeof(server)) < 0) 
	{
		Print("Socket bind failed.\n");
		so_close(sock_fd);
		return;
	}
    
    //receive message from server
    addr_len = 0;
	cli_addr = (struct soaddr_in *)&client;
	send_prcm = (struct remote_communication_msg *)send_buf;
	recv_prcm = (struct remote_communication_msg *)recv_buf;
	
	while(1)
	{
		int recv_bytes;
		
		memset(&recv_buf, 0, REMOTE_COMMUNICATION_PKG_LEN);
		if((recv_bytes = so_recvfrom(sock_fd, recv_buf, REMOTE_COMMUNICATION_PKG_LEN, 0, &client, &addr_len)) < 0)
		{
			Print("The length of received data is less than 0\n");
			continue;
		}
		
		Print("%s,%d\n", recv_prcm->msg, recv_bytes);
		
		switch(recv_prcm->msg_type)
		{
			case REMOTE_COMMUNICATION_MSE_TYPE_QRY:
				is_recv_query = 1;
				send_prcm->msg_type = REMOTE_COMMUNICATION_MSE_TYPE_QRY_ACK;
				sprintf(send_prcm->msg, "Hi %s %u", inet_ntoa(cli_addr->sin_addr), ntohs(cli_addr->sin_port));
				send_prcm->msg_len = strlen(send_prcm->msg);
				break;
			case REMOTE_COMMUNICATION_MSE_TYPE_CFM:
				if (is_recv_query == 0)
				{
					Print("Didn't recive query from %s %u\n", inet_ntoa(cli_addr->sin_addr), ntohs(cli_addr->sin_port));
					continue;
				}
				send_prcm->msg_type = REMOTE_COMMUNICATION_MSE_TYPE_CFM_ACK;
				sprintf(send_prcm->msg, "How do yo do %s %u", inet_ntoa(cli_addr->sin_addr), ntohs(cli_addr->sin_port));
				send_prcm->msg_len = strlen(send_prcm->msg);
				is_recv_query = 0;
				break;
			default:
				continue;
		}
		
		if(so_sendto(sock_fd, send_prcm, sizeof(struct remote_communication_msg) + send_prcm->msg_len, 0, &client, sizeof(client)) < 0)
		{
			Print("Data send failed.\n");
			continue;
		}
	}
    
	so_close(sock_fd);
	
	return;
}

void get_content_from_specify_file(int *file_size, char **ptr_content)
{
	char file_name[FILE_NAME_LEN] = {0};
	
	strncpy(file_name, *ptr_content, FILE_NAME_LEN);
	
	{
		FCB_POINT * fp;
		
		if((fp = file_open(file_name, "r", NULL)) == NULL) 
		{
			sprintf(*ptr_content, "filename %s", file_name);
			*file_size = strlen(*ptr_content);
		
			return;
		}
		
		{
			int f_len = getfilelen(fp);
			if(f_len > BUFFER_SIZE)
				*ptr_content = sys_mem_malloc(f_len + 1);
			
			file_read(fp, *ptr_content, f_len);
			*file_size = f_len;
			
			Print("File size: %d bytes\n", *file_size);
		}
		
		file_close(fp);
	}
	
	return;
}

void os_demo_socket_file(void)
{
    int data_size;
	char *ptr_content;
	
	int sock_fd;
	struct soaddr server, client;   
	struct soaddr_in *serv_addr;

    if ((sock_fd = so_socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
        Print("Creating socket failed.\n");
        return;
    }
	
	serv_addr = (struct soaddr_in *)&server;
	serv_addr->sin_len = sizeof(struct soaddr_in);
	serv_addr->sin_family    = AF_INET;
	serv_addr->sin_port      = htons(SERVER_FILE_PORT);            
	serv_addr->sin_addr.s_addr = INADDR_ANY;
		
	if(so_bind(sock_fd, &server, sizeof(server)) < 0) 
	{
		Print("Socket bind failed.\n");
		return;
	}
	
    while (1) 
	{
		int addrlen = 0;
		char buffer[BUFFER_SIZE];
		memset(&client, 0, sizeof(client));
		
		// recv
		{
			memset(buffer, 0, BUFFER_SIZE);
		
			if(so_recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, &client, &addrlen) < 0)
			{
				Print("Receiving socket failed.\n");
				continue;
			}
		}
		
		{
			ptr_content = buffer;
			get_content_from_specify_file(&data_size, &ptr_content);
		}
		
		// send
		{
			int left_need_to_send = data_size, already_send = 0;
			
			while(left_need_to_send)
			{
				if((already_send += so_sendto(sock_fd, 
					ptr_content + already_send, 
					left_need_to_send > BUFFER_SIZE?BUFFER_SIZE:left_need_to_send, 0, &client, sizeof(client))) < 0)
				{
					Print("Sending data failed.\n");
					break;
				}
				
				left_need_to_send = data_size - already_send;
			}
		}
		
		if(data_size > BUFFER_SIZE)
			sys_mem_free(ptr_content);
    }

    so_close(sock_fd);
	return;
}
