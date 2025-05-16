#include <taskLib.h>
#include <l2/ethernet.h>
#include <ip/socket.h>
#include <ip/inet.h>
#include <ip/ip.h>
#include <ip/ip_icmp.h>

#define MAC_ADDRESS_LEN 6
#define os_demo_mb_forw(m, n) 	(m)->b_rptr += (n); (m)->flen -= (n);
#define os_demo_mb_back(m, n)  	(m)->b_rptr -= (n); (m)->flen += (n);

extern unsigned long Print(char *format, ...);

char *os_demo_mac2str(uint8 *mac)
{
	static char str[32] = {0};

	sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	
	return str;
}

void os_demo_register_cmds(void);
extern unsigned long Print(char *format, ...);
void os_demo_main_task_init(void);

void os_demo_init(void)
{
	os_demo_register_cmds();
	
	os_demo_main_task_init();
	
	return;
}

void os_demo_main_task_init(void)
{
	return;
}

void os_demo_mblk_handle(mblk_t	*mb)
{
	EthHd *eth = NULL;
	struct ip *ip = NULL; 
	struct icmp *icmp = NULL;
	
	if((eth = mtod(mb, EthHd *)) == NULL)
	{
		Print("Invalid Ethernet Header\n");
		return;
	}
	
	Print("\nEthernet Packet\n");
	Print("%20s:%s\n", "Destination Mac", os_demo_mac2str(eth->dmac));
	Print("%20s:%s\n", "Source Mac", os_demo_mac2str(eth->smac));
	
	if (eth->type == ETHER_TYPE_VLAN)
	{
		VlanHd *vhd = NULL;
		if((vhd = mtod(mb, VlanHd *)) == NULL)
		{
			Print("Invalid Ethernet Header\n");
			return;
		}
		
		Print("%20s:%d\n", "Vlan", ntohs(vhd->flag));
		Print("%20s:%s\n", "Type", ntohs(vhd->type) == ETHER_TYPE_IP? "IP":"Others");
		os_demo_mb_forw(mb, sizeof(VlanHd));
	}
	else
	{
		Print("%20s:%s\n", "Type", ntohs(eth->type) == ETHER_TYPE_IP? "IP":"Others");
		os_demo_mb_forw(mb, sizeof(EthHd));
	}
	
	if((ip = mtod(mb, struct ip *)) == NULL)
	{
		Print("Empty IP Header\n");
		return;
	}
	
	Print("IP Packet\n");
	Print("%20s:%d\n", "Header Length", ip->ip_hl * 4);
	Print("%20s:%d\n", "Header Version", ip->ip_v);
	Print("%20s:%d\n", "TOS", ip->ip_tos);
	Print("%20s:%d\n", "Total Length", ntohs(ip->ip_len));
	Print("%20s:%d\n", "Header ID", ntohs(ip->ip_id));
	Print("%20s:%d\n", "Offset", ntohs(ip->ip_off));
	Print("%20s:%d\n", "TTL", ip->ip_ttl);
	Print("%20s:%s\n", "Protocol", ip->ip_p == 1? "ICMP":"Others");
	Print("%20s:%d\n", "Header Checksum", ntohs(ip->ip_sum));
	Print("%20s:%s\n", "Source IP", inet_ntoa(ip->ip_src));
	Print("%20s:%s\n", "Destination IP", inet_ntoa(ip->ip_dst));
	
	os_demo_mb_forw(mb, sizeof(struct ip));
	if((icmp = mtod(mb, struct icmp *)) == NULL)
	{
		Print("Empty IP Header\n");
		return;
	}
	
	Print("ICMP Packet\n");
	Print("%20s:%s\n", "Type", icmp->icmp_type == 8 ? "Echo" : icmp->icmp_type == 0? "Echo Reply" : "Other");
	Print("%20s:%d\n", "Code", icmp->icmp_code);
	Print("%20s:%d\n", "Checksum", ntohs(icmp->icmp_cksum));
	Print("%20s:%d\n", "ID", ntohs(icmp->icmp_id));
	Print("%20s:%d\n", "Sequence", ntohs(icmp->icmp_seq));
	
	return;
}


