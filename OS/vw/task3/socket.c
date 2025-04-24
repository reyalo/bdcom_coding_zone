#include <stdlib.h>

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <global.h>

#include <ip/target.h>
#include <ip/types.h>
#include <ip/in.h>
#include <ip/inet.h>
#include <ip/socket.h>
#include <ip/inet.h>
#include <libcmd/cmdparse.h>
#include <ip/trace.h>
#include <ip/ip_compat.h>
#include <ip/systm.h>
#include <libsys/memory.h>
#if defined(OS_VXWORKS) || defined(OS_LINUX)
#include <libsys/vos/vos_msgq.h>
#include <libsys/vos/vos_task.h>
#endif

#include <ip/ioctl.h>
#include <ip/msg.h>
#include <ip/errno.h>

#define	BUFFER_SIZE 1500

extern void ip_close(void);
int tcp_services(void);

static void udp_echo(void);
static void tcp_client(void);
static void tcp_echo_server(void);
static void str_echo(int s);
static void udp_client(void);

static int do_conf_ip_test_sbcompress(int argc, char **argv, struct user *u);
static int do_conf_ip_test_sbcompress_open(int argc, char **argv, struct user *u);
static int do_conf_ip_test_sbcompress_close(int argc, char **argv, struct user *u);


static struct cmds conf_ip_test_subcmds[] = {
	{ "sbcompress", MATCH_AMB, cmdPref (PF_CMDNO, 0, 0), 0, 
		do_conf_ip_test_sbcompress , NULL, NULL, 0, 0,
		"sbcompress              -- test ip tcp protocol mblk compress functioin",
		"sbcompress              -- ����TCP Э���Mblk �ĺϲ�����",
		NULLCHAR, NULLCHAR
	},

	{NULLCHAR}
} ;

static struct cmds conf_ip_test_sbcompress_subcmds[] = {
	{ "open", MATCH_AMB, 0, 0, 
		do_conf_ip_test_sbcompress_open , NULL, NULL, 0, 0,
		"open              -- start test ip tcp protocol mblk compress functioin",
		"open              -- ��������TCP Э���Mblk �ĺϲ�����",
		NULLCHAR, NULLCHAR
	},

	{ "close", MATCH_AMB, 0, 0, 
		do_conf_ip_test_sbcompress_close , NULL, NULL, 0, 0,
		"close              -- start test ip tcp protocol mblk compress functioin",
		"close              -- ��������TCP Э���Mblk �ĺϲ�����",
		NULLCHAR, NULLCHAR
	},

	{NULLCHAR}
} ;


int ip_test(int argc, char **argv, struct user *u) 
{
	return subcmd(conf_ip_test_subcmds, NULL, argc, argv, u ) ;
}

static int srvfd, clifd ;

int do_conf_ip_test_sbcompress(int argc, char **argv, struct user *u)
{
	return subcmd(conf_ip_test_sbcompress_subcmds, NULL, argc, argv, u ) ;
}

int do_conf_ip_test_sbcompress_open(int argc, char **argv, struct user *u)
{
	int rc ;
	int cliaddr_len ;
	struct soaddr_in srvaddr, cliaddr ;

	srvfd = so_socket( AF_INET, SOCK_STREAM, 0 ) ;
	if ( srvfd <= 0 ) {
		vty_printf( "so_socket error, error code %d\n", srvfd ) ;
		return -1 ;
	}

	memset(&srvaddr, 0, sizeof(srvaddr)) ;
    srvaddr.sin_len = sizeof(srvaddr);
	srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = inet_addr("172.16.20.234");
    srvaddr.sin_port = htons(10001);

    rc = so_bind(srvfd, (struct soaddr *)&srvaddr, sizeof(srvaddr)) ;
    if ( rc ) {
    	vty_printf( "so_bind error, error code %d\n", rc ) ;
		return -1 ;
	}

    rc = so_listen( srvfd, 0 ) ;
    if ( rc ) {
    	vty_printf( "so_listen error, error code %d\n", rc ) ;
		return -1 ;
	}

	memset(&cliaddr, 0, sizeof(cliaddr)) ;
	cliaddr_len = sizeof(cliaddr) ;
    clifd = so_accept( srvfd, (struct soaddr *)&cliaddr, &cliaddr_len) ;
	if ( clifd <= 0 ) {
		vty_printf( "so_accept error, error code %d\n", clifd ) ;
		return -1 ;
	}
	vty_printf( "establish connection from %s succeed\n", inet_ntoa(cliaddr.sin_addr)) ;
	vty_printf_end(1) ;

	return 0 ;
}

int do_conf_ip_test_sbcompress_close(int argc, char **argv, struct user *u)
{
	int rc = 0 ;
	char buff[256] ;

	memset(buff, 0, sizeof(buff)) ;
	rc = so_recv( clifd, buff, sizeof(buff), 0 ) ;
	if ( rc < 0 ) {
		vty_printf( "so_recv error, error code %d\n", rc ) ;
		return -1 ;
	}

	vty_printf( "recv %s succeed\n", buff ) ;

	rc = so_close(clifd) ;
	if ( rc ) {
		vty_printf( "so_close client socket error, error code %d\n", rc ) ;
		return -1 ;
	}
	
	rc = so_close(srvfd) ;
	if ( rc ) {
		vty_printf( "so_close server socket error, error code %d\n", rc ) ;
		return -1 ;
	}

	return 0 ;
}

int tcp_services(void)
{
	ULONG tid;
#if !defined(OS_VXWORKS) && !defined(OS_LINUX)
	ULONG status ;
	status = t_create("UDPE", 128, 4096, 512, 0, &tid);
	assert(status == 0);
	if (status) {
		vty_console_output("%%Warning: UDP echo service failed to initiate.");
		return -1;
	}
	status = t_start(tid, T_SUPV | T_PREEMPT | T_NOTSLICE, udp_echo, 0);
	assert(status == 0);
	if (status) {
		vty_console_output("%%Warning: UDP echo service failed to initiate.");
		return -1;
	}

	status = t_create("TCPE", 128, 4096, 512, 0, &tid);
	assert(status == 0);
	if (status) {
		vty_console_output("%%Warning: TCP echo service failed to initiate.");
		return -1;
	}
	status = t_start(tid, T_SUPV | T_PREEMPT | T_NOTSLICE, tcp_echo_server, 0);
	assert(status == 0);
	if (status) {
		vty_console_output("%%Warning: TCP echo service failed to initiate.");
		return -1;
	}

#else

	tid = sys_task_spawn("UDPE", SYS_TASK_PRI_NORMAL, T_OP_NOPREEMPT, 4096, udp_echo, NULL, 0);
	assert(tid);
	if (tid == 0) {
		vty_console_output("%%Warning: UDP echo service failed to initiate.");
		return -1;
	}
	tid = sys_task_spawn("TCPE", SYS_TASK_PRI_NORMAL, T_OP_NOPREEMPT, 4096, tcp_echo_server, NULL, 0);

	assert(tid);
	if (tid == 0) {
		vty_console_output("%%Warning: TCP echo service failed to initiate.");
		return -1;
	}

#endif

	return 0;
}

int tcpc(int argc, char **argv, struct user *u)
{
	ULONG tid;

#ifdef IP_CODE_UNUSED	/* test */
	printf("min(0xFFFFFFFF, 0xFFFFF000) = 0x%X\n", min(0xFFFFFFFF, 0xFFFFF000));
	printf("umin(0xFFFFFFFF, 0xFFFFF000) = 0x%X\n", umin(0xFFFFFFFF, 0xFFFFF000));
	printf("max(0xFFFFFFFF, 0xFFFFF000) = 0x%X\n", max(0xFFFFFFFF, 0xFFFFF000));
	printf("umax(0xFFFFFFFF, 0xFFFFF000) = 0x%X\n", umax(0xFFFFFFFF, 0xFFFFF000));
	printf("min(0xFFFFFFFF, 0x00FFFFFF) = 0x%X\n", min(0xFFFFFFFF, 0x00FFFFFF));
	printf("umin(0xFFFFFFFF, 0x00FFFFFF) = 0x%X\n", umin(0xFFFFFFFF, 0x00FFFFFF));
	printf("max(0xFFFFFFFF, 0x00FFFFFF) = 0x%X\n", max(0xFFFFFFFF, 0x00FFFFFF));
	printf("umax(0xFFFFFFFF, 0x00FFFFFF) = 0x%X\n", umax(0xFFFFFFFF, 0x00FFFFFF));
	printf("max((int)0xFFFFFFFF, (int)0x00FFFFFF) = 0x%X\n", max((int)0xFFFFFFFF, (int)0x00FFFFFF));
	printf("umax((int)0xFFFFFFFF, (int)0x00FFFFFF) = 0x%X\n", umax((int)0xFFFFFFFF, (int)0x00FFFFFF));
#endif

#if defined(OS_VXWORKS) || defined(OS_LINUX)
	tid = sys_task_spawn("TCPE", SYS_TASK_PRI_NORMAL, T_OP_NOPREEMPT, 10240, tcp_client, NULL, 0);
	assert(tid);

#else
	ULONG status;

	status = t_create("TCPC", 128, 10240, 1024, 0, &tid);
	assert(status == 0);
	status = t_start(tid, T_SUPV | T_PREEMPT | T_NOTSLICE, tcp_client, 0);
	assert(status == 0);
#endif

	return 0;
}


int udpc(int argc, char **argv, struct user *u)
{
	ULONG tid;
#if defined(OS_VXWORKS) || defined(OS_LINUX)
	tid = sys_task_spawn("TCPE", SYS_TASK_PRI_NORMAL, T_OP_NOPREEMPT, 10240, udp_client, NULL, 0);
	assert(tid);
#else
	ULONG status ;

	status = t_create("UDPC", 128, 10240, 1024, 0, &tid);
	assert(status == 0);
	status = t_start(tid, T_SUPV | T_PREEMPT | T_NOTSLICE, udp_client, 0);
	assert(status == 0);
#endif
	return 0;
}


static void
udp_echo(void)
{
	int socketd;
    struct soaddr server, client;    
	struct soaddr_in *servaddr, *cliaddr;	
   	char *buffer;
	int	bytes_sent, bytes_received;
	int	clilen;	
	int udp_buffer_size = 1500;
#ifdef IP_CODE_UNUSED
	int receive = 0, sent = 0;
#endif
#ifdef IP_CODE_UNUSED
	fd_set readfs, writefs, exceptfs;
	struct	timeval tv;
 	int status;   

 	FD_ZERO(&readfs);
    FD_ZERO(&writefs);
    FD_ZERO(&exceptfs);
#endif	
	
	buffer = sys_mem_malloc(udp_buffer_size);
    assert(buffer);
    if (!buffer) {
    	vty_console_output("%%Warning: UDP echo service failed for low memory.");
    	return;
    }
    
   	if ((socketd = so_socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
   		assert(0);
       	return;
    }
	servaddr = (struct soaddr_in *)&server;
	servaddr->sin_len = sizeof(struct soaddr_in);
    servaddr->sin_family    = AF_INET;
    servaddr->sin_port      = htons(7);            
    servaddr->sin_addr.s_addr = INADDR_ANY;

	cliaddr = (struct soaddr_in *)&client;
	cliaddr->sin_len = sizeof(struct soaddr_in);
    cliaddr->sin_family    = AF_INET;
    cliaddr->sin_port      = 0;
    cliaddr->sin_addr.s_addr = 0x0UL;
           
    if (so_bind(socketd, &server, sizeof(server)) < 0) {
		assert(0);
		return;
	}
	
	while (1) {
	
#ifdef IP_CODE_UNUSED
		tv.tv_sec = 300;
		tv.tv_usec = 0;
		
#ifdef IP_CODE_UNUSED
    	FD_CLR(socketd, &writefs);
    	FD_CLR(socketd, &exceptfs);
	    FD_SET(socketd, &readfs);
    	status = so_select(socketd + 1, &readfs, &writefs, &exceptfs, &tv);
      	if (status != 1) {
      		printf("select for readfs returned %d\n", status);
      		break;
      	}
#else 			
 		status = so_setsockopt(socketd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
 		assert(status == 0);
#endif 		

#endif

    	bytes_received = so_recvfrom(socketd, buffer, udp_buffer_size, 0,
    	   							&client, &clilen);     	   							
#ifdef IP_CODE_UNUSED	   					
		TRACE_UDP("%d. Server received %d bytes from %s\n", 
					receive++, bytes_received, inet_ntoa(cliaddr->sin_addr));
#endif	
		assert(bytes_received >= 0);
        if (bytes_received <= 0)
        	continue;

#ifdef IP_CODE_UNUSED	    
	    FD_CLR(socketd, &readfs);
	    FD_CLR(socketd, &exceptfs);
        FD_SET(socketd, &writefs);
        status = so_select(socketd + 1, &readfs, &writefs, &exceptfs, 0);
        if (status != 1) {
           	printf("select error for writefs, returned %d.\n", status);
           	assert(status == 1);
        }
#endif

        bytes_sent = so_sendto(socketd, buffer, bytes_received, 0,
                                &client, clilen);
        assert(bytes_sent == bytes_received);
        
#ifdef IP_CODE_UNUSED                                
		TRACE_UDP("%d. Server sent %d bytes.\n", sent++, bytes_sent);
#endif	
    }
}


static void
udp_client(void)
{
	int socketd;
    struct soaddr server, client;    
	struct soaddr_in *servaddr, *cliaddr;	
   	char *buffer;
	int	bytes_sent;
	int udp_buffer_size = 1000, i = 1;
	
	buffer = sys_mem_malloc(udp_buffer_size);	
	if (NULL == buffer) {
		vty_output("udp_client: sys_mem_malloc(%d) failed.\n", udp_buffer_size);
		return;
	}

   	if ((socketd = so_socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
   		assert(0);
       	return;
    }
    
	servaddr = (struct soaddr_in *)&server;
	servaddr->sin_len = sizeof(struct soaddr_in);
    servaddr->sin_family    = AF_INET;
    servaddr->sin_port      = htons(1001);            
    servaddr->sin_addr.s_addr = inet_addr("192.168.20.43");

	cliaddr = (struct soaddr_in *)&client;
	cliaddr->sin_len = sizeof(struct soaddr_in);
    cliaddr->sin_family    = AF_INET;
    cliaddr->sin_port      = htons(1000);
    cliaddr->sin_addr.s_addr = inet_addr("193.168.0.124");
           
    if (so_bind(socketd, &client, sizeof(client)) < 0) {
		assert(0);
		return;
	}

	if (so_setsockopt(socketd, SOL_SOCKET, SO_RSVP, &i, sizeof(i)) < 0) {
		assert(0);
		return;
	}
	
	while (1) {
    	bytes_sent = so_sendto(socketd, buffer, udp_buffer_size, 0,
    	   							&server, sizeof(server)); 
    	assert(bytes_sent == udp_buffer_size);
		tm_wkafter(100);
    }
}


#define TCP_BUFFER_SIZE 17520

#ifdef IP_CODE_UNUSED
static void
tcp_client(void)
{
    int socketd;
    struct soaddr server;
	struct soaddr_in *servaddr;
    int	status;
    int bytes_received, total_recv = 0;
    int bytes_sent, total_sent = 0;
    char *buffer;       
	int times_recv = 1, times_sent = 1;
	int	i;
	fd_set readfs, writefs, exceptfs;
	unsigned short size = 1000;	
	ULONG error, tid;
	DEVICE_ETERNAL_ID eternalID;
	UINT32 flags;
	DI_NAME_ERROR_PARAM param;
	
	printf("TCP task started.\n");
	FD_ZERO(&readfs);
	FD_ZERO(&writefs);
	FD_ZERO(&exceptfs);
	buffer = sys_mem_malloc(TCP_BUFFER_SIZE);
	if (NULL == buffer) {
		vty_output("tcp_client: sys_mem_malloc(%d) failed.\n", TCP_BUFFER_SIZE);
		return;
	}
	memset(buffer, 'A', TCP_BUFFER_SIZE);
    assert(buffer);   
    servaddr = (struct soaddr_in *)&server;
    servaddr->sin_len = sizeof(*servaddr);
	servaddr->sin_family = AF_INET;
    servaddr->sin_addr.s_addr = inet_addr("192.168.20.124");
    servaddr->sin_port = htons(10000);
        
	if ((socketd = so_socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket() returned %d\n", socketd);
		return;
	}
	printf("socket() succeeded.\n");

	status = interface_omnivorous_callback_newqueryid(-1, "ethernet", "1/1", 0, &flags, &param);
	assert(status == 0);
	eternalID = param.eternalID;
	status = bind_intf(socketd, eternalID);
	assert(status == 0);
		
	if ((i = so_connect(socketd, &server, sizeof(server))) < 0) {
		printf("connect() returned %d\n", i);
		return;
	} 
	printf("connect() succeeded.\n");
    while (bytes_sent >= 0) {
		FD_CLR(socketd, &readfs);
		FD_CLR(socketd, &exceptfs);
		FD_SET(socketd, &writefs);
		status = so_select(socketd + 1, &readfs, &writefs, &exceptfs, 0);
		if (status != 1) {
			printf("select() returned %d\n", status);
			return;
		}
		bytes_sent = so_send(socketd, buffer, size, 0);
	/*	printf("%d. sent %d\n", times_sent, bytes_sent);	*/
        if (bytes_sent < 0) {
			printf("%d. send return %d\n", times_sent, bytes_sent);
            break;
        }
    /*  if (bytes_sent != size)
       		printf("%d. sent %d != size %d\n", times_sent, bytes_sent, size);	*/
        times_sent++;
        total_sent += bytes_sent;
        FD_CLR(socketd, &writefs);
        FD_CLR(socketd, &exceptfs);
        FD_SET(socketd, &readfs);      
        status = so_select(socketd + 1, &readfs, &writefs, &exceptfs, 0);
        if (status != 1) {
        	printf("select() returned %d\n", status);
        	return;
        }
        bytes_received = so_recv(socketd, buffer, BUFFER_SIZE, 0);
	/*	printf("%d. received %d\n", times_recv, bytes_received);	*/
		if (bytes_received < 0) {
			printf("%d. recv return %d\n", times_recv, bytes_received);
           	break;
        }
   	/*  if (bytes_received != bytes_sent)
			printf("%d. received %d != sent %d\n", 
						times_recv, bytes_received, bytes_sent); 	*/
		total_recv += bytes_received;
        times_recv++;
    /* 	printf("Client: sent %d bytes, received %d bytes.\n", 
       			total_sent, total_recv);							*/
  	}
  	printf("Client: sent %d bytes, received %d bytes.\n", 
       			total_sent, total_recv);
    printf("Client is closing.\n");
	if ((so_close(socketd)) != 0)
		printf("close() failure from Client.\n");
    ip_close();
#if defined(OS_VXWORKS) || defined(OS_LINUX)
	tid = sys_task_id_self();
#else
	error = t_ident(0, 0, &tid);
   	assert(error == 0);
#endif
   	printf("t_delete(0x%x)\n", tid);
   	error = t_delete(tid);
   	assert(error == 0);
}
#else
static void
tcp_client(void)
{
    int socketd;
    struct soaddr server, client;
	struct soaddr_in *servaddr, *cliaddr;
    int	status, bytes_received = 0, total_recv = 0, bytes_sent, total_sent = 0;
    char *buffer;       
	int times_recv = 1, times_sent = 1, i = 1, bytes;
	unsigned short size = 1000;
	ULONG error, qid, msg_buf[4];
	DEVICE_ETERNAL_ID eternalID;
	char name[INTERFACE_DEVICE_MAX_NAME], *s = name;
	int valsize = sizeof eternalID;
#ifdef IP_CODE_UNUSED
	UINT32 flags;
	DI_NAME_ERROR_PARAM param;
	int oob = 0;
#endif	
	
	error = q_create((uint32)"TCPC", 200, Q_FIFO | Q_LIMIT, &qid); 
	assert(error == 0);
	
	buffer = sys_mem_malloc(TCP_BUFFER_SIZE);
	if (NULL == buffer) {
		vty_output("tcp_client: sys_mem_malloc(%d) failed.\n", TCP_BUFFER_SIZE);
		return;
	}
	
	memset(buffer, 'A', TCP_BUFFER_SIZE);
  
    servaddr = (struct soaddr_in *)&server;
    servaddr->sin_len = sizeof(*servaddr);
	servaddr->sin_family = AF_INET;
    servaddr->sin_addr.s_addr = inet_addr("192.168.20.124");
    servaddr->sin_port = htons(10001);

	cliaddr = (struct soaddr_in *)&client;
	cliaddr->sin_len = sizeof(*cliaddr);
	cliaddr->sin_family = AF_INET;
	cliaddr->sin_addr.s_addr = 0;
	cliaddr->sin_port = 0;
    
	if ((socketd = so_socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket() returned %d\n", socketd);
		return;
	}
	printf("socket() succeeded.\n");
	
#ifdef IP_CODE_UNUSED
	if ((i = so_bind(socketd, &client, sizeof(client))) < 0) {
		printf("bind() returned %d\n", i);
		return;
	}
	printf("bind() succeeded.\n");
#endif

#ifdef IP_CODE_UNUSED
	/* Without so_bind above, it still works. */
	status = interface_omnivorous_callback_newqueryid(-1, "serial", "0/0", 0, &flags, &param);
	assert(status == 0);
	eternalID = param.eternalID;
	status = bind_intf(socketd, eternalID);
	assert(status == 0);
#endif

	if ((i = so_connect(socketd, &server, sizeof(server))) < 0) {
		printf("connect() returned %d\n", i);
		return;
	}
	printf("connect() succeeded.\n");

	status = so_getsockopt(socketd, SOL_SOCKET, SO_INTERFACE, 
							&eternalID, &valsize);
	assert(status == 0);
	status = interface_omnivorous_callback_newgetname(eternalID, &s);
	assert(status == 0);
	printf("This connecting use interface %s\n", s);
	
	status = socket_register(socketd, qid, 0);
	assert(status == 0);
	status = so_ioctl(socketd, FIONBIO, (char *)&i);
	assert(0 == status);

    while (1) {
#ifdef IP_CODE_UNUSED
    	if (oob == 1) {
    		oob = 0;
	    	bytes_sent = so_send(socketd, buffer, bytes_received, MSG_OOB);
		   	if (bytes_sent <= 0) {
				printf("%d. send return %d\n", times_sent, bytes_sent);
            	goto close;
	        }
	        buffer[bytes_sent] = 0;
	        printf("sent %d byte of OOB data: %s\n", bytes_sent, buffer);
        } else
	    	bytes_sent = so_send(socketd, buffer, size, 0);
#else
		bytes_sent = so_send(socketd, buffer, size, 0);
#endif	    	
	/*	printf("%d. sent %d\n", times_sent, bytes_sent);	*/
       	if (bytes_sent < 0) {
			printf("%d. send return %d\n", times_sent, bytes_sent);
	        break;
        }
	/*  if (bytes_sent != size)
       		printf("%d. sent %d != size %d\n", times_sent, bytes_sent, size);	*/
       	times_sent++;
	    total_sent += bytes_sent;

        error = q_receive(qid, Q_WAIT, 0, msg_buf);
        assert(error == 0);
        switch (msg_buf[0]) {
        case SOCKET_DATARCVD:
	        i = msg_buf[2];
        /*	printf("socket %d received %d bytes\n", msg_buf[1], i);	*/
           	assert(msg_buf[1] == socketd && i >= 0);
        	status = so_ioctl(socketd, FIONREAD, (char *)&bytes);
        	assert(0 == status);
        	bytes_received = so_recv(socketd, buffer, i, 0);
           	break;

        case SOCKET_CONNECTED:
        	printf("SOCKET_CONNECTED\n");
        	break;

        case SOCKET_DISCONNECTING:
        	printf("SOCKET_DISCONNECTING\n");
        	break;

        case SOCKET_DISCONNECTED:
        	printf("SOCKET_DISCONNECTED\n");
        	goto close;
        	
        case SOCKET_CANTSENDMORE:
        	printf("SOCKET_CANTSENDMORE\n");
        	break;

        case SOCKET_CANTRCVMORE:
        	printf("SOCKET_CANTRCVMORE\n");
        	break;

		case SOCKET_OOBDATARCVD:
			printf("SOCKET_OOBDATARCVD\n");
			while ((bytes_received = so_recv(socketd, buffer, TCP_BUFFER_SIZE, MSG_OOB)) < 0) {
				if (ERR_WOULDBLOCK == bytes_received) {
					bytes_received = so_recv(socketd, buffer, TCP_BUFFER_SIZE, 0);
					if (bytes_received <= 0)
						return;
				} else
					break;
			}

			if (bytes_received <= 0)
				break;
			
			buffer[bytes_received] = 0;
			printf("recv %d OOB byte: %s\n", bytes_received, buffer);
			total_recv += bytes_received;
	        times_recv++;			
	        
#ifdef IP_CODE_UNUSED   
    	    oob = 1;
#else
			bytes_sent = so_send(socketd, buffer, bytes_received, MSG_OOB);
		   	if (bytes_sent <= 0) {
				printf("%d. send return %d\n", times_sent, bytes_sent);
            	goto close;
	        }
	        buffer[bytes_sent] = 0;
	        printf("sent %d byte of OOB data: %s\n", bytes_sent, buffer);
#endif
        	break;
        	
        default:
        	printf("Unknow message \n");
        	break;
        }

        if (msg_buf[0] != SOCKET_DATARCVD)
        	continue;
        
	/*	printf("%d. received %d\n", times_recv, bytes_received);	*/
		if (bytes_received <= 0) {
			printf("%d. recv return %d\n", times_recv, bytes_received);
           	break;
        }
   	/*  if (bytes_received != bytes_sent)
			printf("%d. received %d != sent %d\n", 
						times_recv, bytes_received, bytes_sent); 	*/

		size = (unsigned short)bytes_received;
		total_recv += bytes_received;
        times_recv++;
        
    /* 	printf("Client: sent %d bytes, received %d bytes.\n", 
       			total_sent, total_recv);							*/
  	}
close:
  	printf("Client: sent %d bytes, received %d bytes.\n", 
       			total_sent, total_recv);
    printf("Client is closing.\n");
	if ((so_close(socketd)) != 0)
		printf("close() failure from Client.\n");
/* 	ip_close();	*/
}
#endif


static void
tcp_echo_server(void)
{
	int socketd, newsock;
    struct soaddr	server;    
	struct soaddr_in	*servaddr;	
    int status;
    struct soaddr	client_addr;   
   	int addrlen;
	fd_set readfs, writefs, exceptfs;
#ifdef IP_CODE_UNUSED
	struct timeval tv;
#endif
	ULONG targs[4], tid;

	FD_ZERO(&readfs);
	FD_ZERO(&writefs);
	FD_ZERO(&exceptfs);
	socketd = so_socket(AF_INET, SOCK_STREAM, 0);
	if (socketd < 0) {
		assert(0);
		return;
	}
	servaddr = (struct soaddr_in *)&server;
	servaddr->sin_family = AF_INET;
	servaddr->sin_port = htons(7);
	servaddr->sin_len = sizeof(struct soaddr);
	servaddr->sin_addr.s_addr = INADDR_ANY;
	if (so_bind(socketd, &server, sizeof(struct soaddr))) {
		assert(0);
		return;
	}		
	if (so_listen(socketd, 5)) {
		assert(0);
		return;
	}
	addrlen = sizeof(server);
	FD_SET(socketd, &readfs);
#ifdef IP_CODE_UNUSED
	tv.tv_sec = 0;
	tv.tv_usec = 1000;	
#endif

    while (1) {
    	status = so_select(socketd + 1, &readfs, &writefs, &exceptfs, NULL);
	    if (status != 1) {
    	   	assert(0);
    	   	continue;
	    }
		if ( (newsock = so_accept(socketd, &client_addr, &addrlen)) < 0 ) {
			assert(0);
			continue;
		}
    	targs[0] = newsock;

#if defined(OS_VXWORKS) || defined(OS_LINUX)
		tid = sys_task_spawn("TCP_", SYS_TASK_PRI_NORMAL, T_OP_NOPREEMPT, 4096, str_echo, targs, 1);
#else
		status = t_create("TCP_", 128, 4096, 512, 0, &tid);
    	assert(status == 0);
	    status = t_start(tid, T_SUPV | T_PREEMPT | T_NOTSLICE, str_echo, targs);
    	assert(status == 0);
#endif
	}
}

void str_echo(int sockfd)
{
    int	bytes_recv, total_recv = 0;
	int	bytes_sent, total_sent = 0;
    int	status;    	
	char *buffer;
#ifdef IP_CODE_UNUSED
	int times_recv = 1, times_sent = 1;
#endif
	
	buffer = sys_mem_malloc(TCP_BUFFER_SIZE);
	if (NULL == buffer) {		
		status = so_close(sockfd);
		assert(0 == status);
		return;
	}
	
    while (1) {		

#if 1
    	bytes_recv = so_recv(sockfd, buffer, TCP_BUFFER_SIZE, 0);			
        if (bytes_recv <= 0) {
#ifdef IP_CODE_UNUSED
			printf("Server recv fail: %d\n", bytes_recv);
#endif
			break;
		}		
#ifdef IP_CODE_UNUSED
		printf("%d. recv %d bytes.\n", times_recv++, bytes_recv);		
#endif	
		total_recv += bytes_recv;
		
#else
		bytes_recv = 1000;
#endif

        bytes_sent = so_send(sockfd, buffer, bytes_recv, 0);

		if (bytes_sent < 0) {
#ifdef IP_CODE_UNUSED
			printf("Server send fail: %d\n", bytes_sent);
#endif
			break;
	    } else     
	        total_sent += bytes_sent;
	        
#ifdef IP_CODE_UNUSED
		printf("%d. send %d bytes.\n", times_sent++, bytes_sent);
		if(bytes_sent != bytes_recv)
			printf("   bytes_sent %d != bytes_recv %d.\n", bytes_sent, bytes_recv);
#endif
    }
    
#ifdef IP_CODE_UNUSED   
    printf("Server: received %d bytes, sent %d bytes\n", total_recv, total_sent);
#endif

    status = so_close(sockfd);
    assert(0 == status);
}

