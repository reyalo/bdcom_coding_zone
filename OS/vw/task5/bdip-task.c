#include "bdip.h"

extern void bdip_client_send(void);
extern void bdip_recv_socket_init(void);
extern int bdip_client_packet_input_from_ip(void);
extern void bdip_try_send(void);
extern void bdip_server_send_query(void);
extern bool bdip_mqtt_send_own_info(void);
extern int bdip_server_report_handler(void);
void mqtt_discover_timo_handler();

void bdip_main(void)
{
	uint32 msgbuf[4];
	bool rv = FALSE;
	
	while (1) 
	{
		/* wait message from queue forever */
		q_receive(bdip_global_qid, Q_WAIT, 0, (unsigned long *)msgbuf);
		switch (msgbuf[3])
		{
			/* received packet from socket */
			case BDIP_CLIENT_CODE_AGENT_QUERY:
				/* process incoming udp socket data */
				if (bdip_info.bdip_client_enabled)
					bdip_client_packet_input_from_ip();
				break;
			case BDIP_CLIENT_CODE_UNSOLICITED_REPORT:
				
				if (bdip_info.bdip_client_enabled)
					bdip_client_send();
				
				bdip_send_count++;

				if (bdip_send_count >= BDIP_UNSLCTD_RETRY)
				{
					bdip_send_count = 0;
					if (bdip_send_tmid)
					{
						sys_delete_timer(bdip_send_tmid);
						bdip_send_tmid = 0;
					}
				}
				
				break;
			case BDIP_TRY_SEND:
				bdip_try_send();
				break;
			case BDIP_SERVER_CODE_AGENT_QUERY:	
				if (redn_msu_is_active())
				{
					if(bdip_info.bdip_server_enabled)
					{
						if(bdip_info.bdip_server_mode == BDIP_SERVER_MODE_MQTT)
						{
							if(bdip_info.bdip_server_count == 0)
							{	
								if (bdip_mqtt_tmid != 0)
								{
									sys_delete_timer(bdip_mqtt_tmid);
									bdip_mqtt_tmid = 0;
								}
							}
							else
								bdip_info.bdip_server_count--;
						}

						bdip_server_send_query();

						if(bdip_info.bdip_client_enabled && (bdip_info.bdip_server_mode == BDIP_SERVER_MODE_MQTT))
							bdip_mqtt_send_own_info();

					}
				}
				break;
			case BDIP_SERVER_CODE_REPORT:
				if (redn_msu_is_active())
				{
					if(bdip_info.bdip_server_enabled)
						bdip_server_report_handler();
				}			
				break;
				
            default:
                break;
		}
		tm_wkafter(0);
	}
	return;
}

void bdip_client_timer(void)
{
	int status;
	TIMER_USER_DATA ud;	

	if (bdip_send_tmid)
		return;
	ud.msg.qid = bdip_global_qid;
	ud.msg.msg_buf[3] = BDIP_CLIENT_CODE_UNSOLICITED_REPORT;
	sys_add_timer(TIMER_LOOP|TIMER_MSG_METHOD, &ud, &bdip_send_tmid);
	status = sys_start_timer(bdip_send_tmid, BDIP_UNSLCTD_INTERVAL | TIMER_RESOLUTION_S);
	
	if (status)
	{
		syslog(LOG_WARNING, "%%BDIP-ERROR failed to start client timer\n");
	}
	return;
}

void bdip_server_timer(void)
{
	int status;
	TIMER_USER_DATA ud;	

	if (bdip_server_tmid)
		return;
	
	ud.msg.qid = bdip_global_qid;
	ud.msg.msg_buf[3] = BDIP_SERVER_CODE_AGENT_QUERY;
	sys_add_timer(TIMER_LOOP|TIMER_MSG_METHOD, &ud, &bdip_server_tmid);
	status = sys_start_timer(bdip_server_tmid, BDIP_AGENT_QUERY_INTERVAL | TIMER_RESOLUTION_S);
	
	if (status)
		syslog(LOG_WARNING, "%%BDIP-ERROR failed to start server timer\n");
	
	return;
}

void bdip_mqtt_timer(void)
{
	int status;
	TIMER_USER_DATA ud;	

	if (bdip_mqtt_tmid)
		return;
	
	ud.msg.qid = bdip_global_qid;
	ud.msg.msg_buf[3] = BDIP_SERVER_CODE_AGENT_QUERY;
	sys_add_timer(TIMER_LOOP|TIMER_MSG_METHOD, &ud, &bdip_mqtt_tmid);
	status = sys_start_timer(bdip_mqtt_tmid, bdip_info.bdip_server_interval | TIMER_RESOLUTION_S);
	
	if (status)
		syslog(LOG_WARNING, "%%BDIP-ERROR failed to start mqtt timer\n");
	
	return;
}

void bdip_mqtt_discover_timer(int remain_ti)
{
	int status;
	TIMER_USER_DATA ud; 

	if (bdip_mqtt_remain_tmid)
		return;
	
	ud.cb.fun = mqtt_discover_timo_handler;
	ud.cb.arg = 0;
	sys_add_timer(TIMER_CALLBACK_METHOD, &ud, &bdip_mqtt_remain_tmid);
	status = sys_start_timer(bdip_mqtt_remain_tmid, remain_ti | TIMER_RESOLUTION_S);

	if (status)
		syslog(LOG_WARNING, "%%BDIP-ERROR failed to start remain timer\n");

	return;
}

void mqtt_discover_timo_handler(void)
{
	if(bdip_info.bdip_server_enabled && (bdip_info.bdip_server_mode == BDIP_SERVER_MODE_MQTT))
	{
		if (bdip_mqtt_tmid != 0)
		{
			sys_delete_timer(bdip_mqtt_tmid);
			bdip_mqtt_tmid = 0;
		}
		
		bdip_info.bdip_server_mode = BDIP_SERVER_MODE_CMD;
		bdip_server_timer();
	}

	if (bdip_mqtt_remain_tmid != 0)
	{
		sys_delete_timer(bdip_mqtt_remain_tmid);
		bdip_mqtt_remain_tmid = 0;
	}
	
	return;
}

void bdip_client(void)
{
	int rc;
	struct snmp_cop_info *snmp_info_first;
	
	bdip_recv_socket_init();
	bdip_client_global_sendsocket = so_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	
	bdip_client_global_respsocket = so_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	

	snmp_info_first = get_snmp_cop_info();
	memcpy(&snmp_info, snmp_info_first, sizeof(struct snmp_cop_info));
	
	rc = socket_register(bdip_client_global_recvsocket, bdip_global_qid, BDIP_CLIENT_CODE_AGENT_QUERY);

	if (rc)
	{
		syslog(LOG_WARNING, "%%BDIP-ERROR failed to start receive agent query\n");
	}

	return;
}

void bdip_server(void)
{
	int rc;
	int ret;
	struct soaddr_in sin;

	bdip_server_global_recvsocket = so_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* bind address */
	sin.sin_family = AF_INET;
	sin.sin_len = sizeof(struct soaddr_in);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(BDIP_SERVER_RECEIVE_PORT);
	ret = so_bind(bdip_server_global_recvsocket, (struct soaddr *)&sin, sizeof(sin));
	assert(ret>=0);
	
	bdip_server_global_sendsocket = so_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	
	bdip_server_global_respsocket = so_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	

	rc = socket_register(bdip_server_global_recvsocket, bdip_global_qid, BDIP_SERVER_CODE_REPORT);

	if (rc)
		syslog(LOG_WARNING, "%%BDIP-ERROR failed to start receive report\n");

	return;
}

