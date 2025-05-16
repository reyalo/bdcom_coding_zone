/*
 * Fundamental constants relating to ethernet.
 *
 * $FreeBSD: src/sys/net/ethernet.h,v 1.11 1999/11/05 14:41:29 shin Exp $
 *
 */

#ifndef _ETHERNET_H_
#define _ETHERNET_H_
#include <ip/in.h>

/*OUI*/
#define OUI_OF_BDCOM 		{0x00, 0xe0, 0x0f}
#define OUI_OF_CISCO_IN_OAM	{0x00, 0x00, 0x0c}	/*Ciscoע���˺ܶ�OUI��������OUI��00000c����С�ĺ��룬����OAM������ʹ�õ�Ҳ��00000c��*/

/*
 * The number of bytes in an ethernet (MAC) address.
 */
#define	ETHER_ADDR_LEN		6
#define L2_MAC_ADDRESS_LEN  ETHER_ADDR_LEN

#define	ETHER_VLAN_TAG_LEN     4
#define L2_VLAN_TAG_LEN     4

/*
 * The number of bytes in the type field.
 */
#define	ETHER_TYPE_LEN		2

/*
 * The number of bytes in the trailing CRC field.
 */
#define	ETHER_CRC_LEN		4

/*
 * The length of the combined header.
 */
#define	ETHER_HDR_LEN		(ETHER_ADDR_LEN*2+ETHER_TYPE_LEN)

/*
 * The minimum packet length.
 */
#define	ETHER_MIN_LEN		64

/*
 * The maximum packet length.
 */
#define	ETHER_MAX_LEN		1518

#define	ETHERMTU	(ETHER_MAX_LEN-ETHER_HDR_LEN-ETHER_CRC_LEN)
#define	ETHERMIN	(ETHER_MIN_LEN-ETHER_HDR_LEN-ETHER_CRC_LEN)

#define LLC_SANP_HDR_LEN	0x08

/*
 * define the EtherNet Packet Type All Known
 */
#define	ETHER_TYPE_PUP          0x0200 /* PUP protocol, EtherNet Type Field */
#define	ETHER_TYPE_IP           0x0800 /* IP protocol, EtherNet Type Field */
#define	ETHER_TYPE_ARP          0x0806 /* Addr. resolution protocol, EtherNet Type Field */
#define	ETHER_TYPE_CDP          0x2000 /* CISCO Discovery Protocol, snap type field */
#define ETHER_TYPE_LLDP         0x88CC /* Link Layer Discovery Protocol , EtherNet type field */ 
#define ETHER_TYPE_UDLD         0x0111 /* UniDirectional Link Detection, EtherNet type field*/
#define	ETHER_TYPE_VTP          0x2003 /* Virtual LAN Trunk Protocol, snap type field*/
#define	ETHER_TYPE_PVST         0x010B /* PerVLAN Spanning-Tree Protocol, snap type field*/
#define ETHER_TYPE_BACKUPLINK   0x0127 /* Cisco FlexLink PDU */
#define	ETHER_TYPE_REVARP       0x8035 /* reverse Addr. resolution protocol */
#define	ETHER_TYPE_VLAN         0x8100 /* IEEE 802.1Q VLAN tagging, EtherNet Type Field */
#define	ETHER_TYPE_IPV6         0x86dd /* IPv6 , EtherNet Type Field */
#define	ETHER_TYPE_DOT1X        0x888E /* IEEE 802.1x , EtherNet Type Field */
#define ETHER_TYPE_SLOW		    0x8809 /*slow protocol include lacp and oam*/
#define	ETHER_TYPE_LACP         ETHER_TYPE_SLOW	/* IEEE 802.3AD LACP, EtherNet Type Field, a kind of slow protocol*/
#define ETHER_TYPE_PTP		    0x88f7 /* IEEE 1588, Precision clock synchronization protocol*/
#define	ETHER_TYPE_LOOPBACK     0x9000 /* used to test interfaces */
#define	ETHER_TYPE_TRAIL	    0x1000		/* Trailer packet */
#define	ETHER_TYPE_BCMP		    0xF000		/* Trailer packet */
#define	ETHER_TYPE_EBCMP	    0x0877	
#define ETHER_TYPE_EDDP			0x88B7
#define ETHER_TYPE_PSG          0x8810
#define ETHER_TYPE_MRP		    0x88E3 /* MRP protocol */
#define ETHER_TYPE_TMRP		    0x9126 /* TMRP protocol */
#define ETHER_TYPE_TRILL_DATA   0x22F3 /* TRILL DATA frame */
#define ETHER_TYPE_TRILL_CTRL   0x22F4 /* TRILL protocol*/
#define ETHER_TYPE_FIP          0x8914 
#define ETHER_TYPE_FCOE         0x8906 
#define ETHER_TYPE_PRP	        0x88FB /* IEC 62439-3 PRP/HSR ptococol */
#define ETHER_TYPE_JUMBO        0x8870 /* Ϊ��˼�Ƶ�ISIS�ԽӶ����� */
#define	ETHER_TYPE_NTRAILER	    16

#define SLOW_SUBTYPE_LACP	0x01
#define SLOW_SUBTYPE_OAM	0x03
#define SLOW_SUBTYPE_ESMC	0x0A

/*
 * The ETHERTYPE_NTRAILER packet types starting at ETHERTYPE_TRAIL have
 * (type-ETHERTYPE_TRAIL)*512 bytes of data followed
 * by an ETHER type (as given above) and then the (variable-length) header.
*/
#define ETHER_TYPE_MPLS_UNICAST 	0x8847 					/* MPLS Label Stack Encoding: MPLS Unicast */
#define ETHER_TYPE_MPLS				ETHER_TYPE_MPLS_UNICAST /* Module Type */
#define ETHER_TYPE_MPLS_MULTICAST	0x8848 					/* MPLS Multicast */

#define ETHER_TYPE_CFM				0x8902					/* IEEE 802.1ag Connectivity Fault Management */

#define ETHER_TYPE_ERPS				0x7001					/* Ethernet Ring Portection Switching Discovery */
#define ETHER_TYPE_PPPOEDISCOVERY   0x8863					/* pppoe discovery */
#define ETHER_TYPE_LOOPBACK_DETECT  0x9001                  /* Loopback Detect */
#define ETHER_TYPE_CTRLRELAY		0xC0C0					/* ctrlrleay icc */
#define ETHER_TYPE_STACKING			0xC1C1					/* stacking mangment */
#define ETHER_TYPE_STK_SENDMSU		0xC2C2	/* �����ڶѵ�ϵͳ�ж�ͨ��INTERFACE_LINK_SERVICE_SEND_MSU���͵ı��Ľ��з�װ */
#define ETHER_TYPE_IPWC				0x0808

#if (CPU_FAMILY == ARM) || (CPU_FAMILY == MIPS)
/* compare two Mac Address */
#define MAC_ADDR_CMP(x,y)	 ( *(uint16 *)(x) == *(uint16 *)(y) &&\
	*(uint16*)( ((uint8*)x)+ 2 ) == *(uint16*)( ((uint8*)y) + 2 ) &&\
	*(uint16*)( ((uint8*)x)+ 4 ) == *(uint16*)( ((uint8*)y) + 4 ))

#define MAC_ADDR_CPY(dst,src)	 { *(uint16 *)(dst) = *(uint16 *)(src);\
	*(uint16*)( ((uint8*)dst)+ 2 ) = *(uint16*)( ((uint8*)src) + 2 );\
	*(uint16*)( ((uint8*)dst)+ 4 ) = *(uint16*)( ((uint8*)src) + 4 );}

#define MAC_ADDR_SET_ZERO(x)	 { *(uint16 *)(x) = 0 ; \
	*(uint16*)( ((uint8*)x)+ 2 ) = 0 ; *(uint16*)( ((uint8*)x)+ 4 ) = 0 ; }

#define MAC_ADDR_SET_IPMULTICAST(ipaddr, enaddr) \
	/* IP MutlCast Address ; */ \
	/* struct in_addr *ipaddr; */ \
	/* u_char enaddr[ETHER_ADDR_LEN];	   */ \
{ \
	(enaddr)[0] = 0x01; \
	(enaddr)[1] = 0x00; \
	(enaddr)[2] = 0x5e; \
	(enaddr)[3] = ((u_char *)ipaddr)[1] & 0x7f; \
	(enaddr)[4] = ((u_char *)ipaddr)[2]; \
	(enaddr)[5] = ((u_char *)ipaddr)[3]; \
}

#define MAC_ADDR_SET_IPBROADCAST(x)    	{\
		(*(uint16*)(x)) = ntohs(0xFFFF); \
		(*(uint16*)(x + 2)) = ntohs(0xFFFF);\
		(*(uint16*)(x + 4)) = ntohs(0xFFFF); }

#define MAC_ADDR_SET_IPV6MULTICAST(ip6addr, enaddr)			\
	do{																		\
		(enaddr)[0] = 0x33; 					\
		(enaddr)[1] = 0x33; 					\
		(enaddr)[2] = ((u_char *)ip6addr)[12];				\
		(enaddr)[3] = ((u_char *)ip6addr)[13];				\
		(enaddr)[4] = ((u_char *)ip6addr)[14];				\
		(enaddr)[5] = ((u_char *)ip6addr)[15];				\
	}while(0)

#define MAC_ADDR_IPMULTICAST(x)  ((ntohs(*(uint16*)(x)) & 0xffff) == 0x0100 && (ntohs(*(uint16*)(x + 2)) & 0xff00) == 0x5e00 )
#define MAC_ADDR_MULTICAST(x)    ( *(uint8*)(x) & 0x01 )
#define MAC_ADDR_IPV6MULTICAST(x)  ((ntohs(*(uint16*)(x)) & 0xffff) == 0x3333)
#define MAC_ADDR_BROADCAST(x)    ( (ntohs(*(uint16*)x) == 0xFFFF ) && \
	(ntohs(*(uint16*)(x + 2)) == 0xFFFF ) && (ntohs(*(uint16*)(x + 4)) == 0xFFFF ) )
#define MAC_ADDR_ZERO(x)   ( (ntohs(*(uint16*)(x)) == 0x0000 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x0000 ) && (ntohs(*(uint16*)(x + 4)) == 0x0000 ) )
#define MAC_ADDR_CDP(x)    ( (ntohs(*(uint16*)(x)) == 0x0100 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x0CCC ) && \
	(ntohs(*(uint16*)(x + 4)) == 0xCCCC ) )
#define MAC_ADDR_STP(x)    ((ntohs(*(uint16*)(x)) == 0x0180 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0xC200 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0000 ) )
#define MAC_ADDR_GVRP(x)   ( (ntohs(*(uint16*)(x)) == 0x0180 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0xC200 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0021 ))
#define MAC_ADDR_GMRP(x)   ( (ntohs(*(uint16*)(x)) == 0x0180 ) && \
     (ntohs(*(uint16*)(x + 2)) == 0xC200 ) && \
     (ntohs(*(uint16*)(x + 4)) == 0x0020 ) )
#define MAC_ADDR_SLOW(x)   ( (ntohs(*(uint16*)(x)) == 0x0180 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0xC200 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0002 ) )
#define MAC_ADDR_DOT1X(x)  ( (ntohs(*(uint16*)(x)) == 0x0180 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0xC200 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0003 ) )
#define MAC_ADDR_PVST(x)   ( (ntohs(*(uint16*)(x)) == 0x0100 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x0CCC ) && \
	(ntohs(*(uint16*)(x + 4)) == 0xCCCD ) )
#define MAC_ADDR_BDCOM_UC_1(x)   ( (ntohs(*(uint16*)(x)) == 0x00E0 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x0F00 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0001 ) )
#define MAC_ADDR_EPXRING(x)       ( (ntohs(*(uint16*)(x)) == 0x0180 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0xC200 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0006 ) )
#define MAC_ADDR_BDCOM_UC_2(x)   ( (ntohs(*(uint16*)(x)) == 0x00E0 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x0F00 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0002 ) )
#define MAC_ADDR_BDCOM_MC_1(x)   ( (ntohs(*(uint16*)(x)) == 0x01E0 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x0F00 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0001 ) )
#define MAC_ADDR_EAPS_XTREM(x)   ( (ntohs(*(uint16*)(x)) == 0x00E0 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x2B00 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0004 ) )
#define MAC_ADDR_MRP_TEST(x)     ( (ntohs(*(uint16*)(x)) == 0x0115 ) && \
		(ntohs(*(uint16*)(x + 2)) == 0x4E00 ) && \
		(ntohs(*(uint16*)(x + 4)) == 0x0001 ) )
#define MAC_ADDR_MRP_TOPOLOGY(x) ( (ntohs(*(uint16*)(x)) == 0x0115 ) && \
		(ntohs(*(uint16*)(x + 2)) == 0x4E00 ) && \
		(ntohs(*(uint16*)(x + 4)) == 0x0002 ) )
/* All Level 1 Intermediate System Address */
#define MAC_ADDR_ALLLV1IS(x)	( (ntohs(*(uint16*)(x)) == 0x0180 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0xC200 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0014 ) )
/* All Level 2 Intermediate System Address */
#define MAC_ADDR_ALLLV2IS(x)	( (ntohs(*(uint16*)(x)) == 0x0180 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0xC200 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0015 ) )
/* All End System Network Entities Address */
#define MAC_ADDR_ALLENDSYSNE(x)	( (ntohs(*(uint16*)(x)) == 0x0900 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x2B00 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0004 ) )
/* All Intermediate System Network Entities Address */
#define MAC_ADDR_ALLISNE(x)	( (ntohs(*(uint16*)(x)) == 0x0900 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x2B00 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0005 ) )
#define MAC_ADDR_VTP      MAC_ADDR_CDP
#define MAC_ADDR_FWDPROTO(x) ((ntohs(*(uint16*)(x))  == 0x0180) && (ntohs(*(uint16*)(x + 2))  == 0xc200))
#define MAC_ADDR_L2PTUNNEL(x)    (	(ntohs(*(uint16*)(x)) == 0x0100 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x0CCD ) && \
	(ntohs(*(uint16*)(x + 4)) == 0xCDD0 ) )
#define MAC_ADDR_ERPS(x)	( (ntohs(*(uint16*)(x)) == 0x0119 ) && \
	(ntohs(*(uint16*)(x + 2)) == 0xA700 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0001 ) )
#define MAC_ADDR_PTP(x)		( (ntohs(*(uint16*)(x)) == 0x011b ) && \
	(ntohs(*(uint16*)(x + 2)) == 0x1900 ) && \
	(ntohs(*(uint16*)(x + 4)) == 0x0000 ) )
#define MAC_ADDR_STACK_SEND_MSU(x)	( (ntohs(*(uint16*)(x)) == 0x00ff ) && \
									(ntohs(*(uint16*)(x + 2)) == 0xffff ) && \
									(ntohs(*(uint16*)(x + 4)) == 0xffff ) )

#else
/* compare two Mac Address */
#define MAC_ADDR_CMP(x,y)	 ( *(uint32 *)(x) == *(uint32 *)(y) &&\
	*(uint16*)( ((uint8*)x)+ 4 ) == *(uint16*)( ((uint8*)y) + 4 ))

#define MAC_ADDR_CPY(dst,src)	 { *(uint32 *)(dst) = *(uint32 *)(src);\
	*(uint16*)( ((uint8*)dst)+ 4 ) = *(uint16*)( ((uint8*)src) + 4 );}

#define MAC_ADDR_SET_ZERO(x)	 { *(uint32 *)(x) = 0 ; \
	*(uint16*)( ((uint8*)x)+ 4 ) = 0 ; }

#define MAC_ADDR_SET_IPMULTICAST(ipaddr, enaddr) \
	/* IP MutlCast Address ; */ \
	/* struct in_addr *ipaddr; */ \
	/* u_char enaddr[ETHER_ADDR_LEN];	   */ \
{ \
	(enaddr)[0] = 0x01; \
	(enaddr)[1] = 0x00; \
	(enaddr)[2] = 0x5e; \
	(enaddr)[3] = ((u_char *)ipaddr)[1] & 0x7f; \
	(enaddr)[4] = ((u_char *)ipaddr)[2]; \
	(enaddr)[5] = ((u_char *)ipaddr)[3]; \
}

#define MAC_ADDR_SET_IPBROADCAST(x)    	{\
		(*(uint32*)(x + 2)) = ntohl(0xFFFFFFFF);\
		(*(uint16*)(x)) = ntohs(0xFFFF); }

#define MAC_ADDR_SET_IPV6MULTICAST(ip6addr, enaddr)			\
	do{																		\
		(enaddr)[0] = 0x33; 					\
		(enaddr)[1] = 0x33; 					\
		(enaddr)[2] = ((u_char *)ip6addr)[12];				\
		(enaddr)[3] = ((u_char *)ip6addr)[13];				\
		(enaddr)[4] = ((u_char *)ip6addr)[14];				\
		(enaddr)[5] = ((u_char *)ip6addr)[15];				\
	}while(0)

#define MAC_ADDR_IPMULTICAST(x)  ((ntohl(*(uint32*)(x)) & 0xffffff00) == 0x01005e00)
#define MAC_ADDR_IPV6MULTICAST(x)  ((ntohl(*(uint32*)(x)) & 0xffff0000) == 0x33330000)
#define MAC_ADDR_MULTICAST(x)    ( ntohl(*(uint32*)(x)) & 0x01000000 )
#define MAC_ADDR_BROADCAST(x)    ( (ntohl(*(uint32*)(x + 2)) == 0xFFFFFFFF ) && \
	(ntohs(*(uint16*)(x)) == 0xFFFF ) )
#define MAC_ADDR_ZERO(x)   ( (ntohl(*(uint32*)(x + 2)) == 0x00000000 ) && \
	(ntohs(*(uint16*)(x)) == 0x0000 ) )
#define MAC_ADDR_CDP(x)    ( (ntohl(*(uint32*)(x + 2)) == 0x0CCCCCCC ) && \
	(ntohs(*(uint16*)(x)) == 0x0100 ) )
#define MAC_ADDR_STP(x)    ( (ntohl(*(uint32*)(x + 2)) == 0xC2000000 ) && \
	(ntohs(*(uint16*)(x)) == 0x0180 ) )
#define MAC_ADDR_GVRP(x)   ( (ntohl(*(uint32*)(x + 2)) == 0xC2000021 ) && \
	(ntohs(*(uint16*)(x)) == 0x0180 ) )
#define MAC_ADDR_GMRP(x)   ( (ntohl(*(uint32*)(x + 2)) == 0xC2000020 ) && \
     (ntohs(*(uint16*)(x)) == 0x0180 ) )
#define MAC_ADDR_SLOW(x)   ( (ntohl(*(uint32*)(x + 2)) == 0xC2000002 ) && \
	(ntohs(*(uint16*)(x)) == 0x0180 ) )
#define MAC_ADDR_DOT1X(x)  ( (ntohl(*(uint32*)(x + 2)) == 0xC2000003 ) && \
	(ntohs(*(uint16*)(x)) == 0x0180 ) )
#define MAC_ADDR_PVST(x)   ( (ntohl(*(uint32*)(x + 2)) == 0x0CCCCCCD ) && \
	(ntohs(*(uint16*)(x)) == 0x0100 ) )
#define MAC_ADDR_BDCOM_UC_1(x)   ( (ntohl(*(uint32*)(x + 2)) == 0x0F000001 ) && \
	(ntohs(*(uint16*)(x)) == 0x00E0 ) )
#define MAC_ADDR_EPXRING(x)       ( (ntohl(*(uint32*)(x +2)) == 0xC2000006 ) && \
	(ntohs(*(uint16*)(x)) == 0x0180 ) )
#define MAC_ADDR_BDCOM_UC_2(x)   ( (ntohl(*(uint32*)(x + 2)) == 0x0F000002 ) && \
	(ntohs(*(uint16*)(x)) == 0x00E0 ) )
#define MAC_ADDR_BDCOM_MC_1(x)   ( (ntohl(*(uint32*)(x + 2)) == 0x0F000001 ) && \
	(ntohs(*(uint16*)(x)) == 0x01E0 ) )
#define MAC_ADDR_EAPS_XTREM(x)   ( (ntohl(*(uint32*)(x + 2)) == 0x2B000004 ) && \
	(ntohs(*(uint16*)(x)) == 0x00E0 ) )
#define MAC_ADDR_MRP_TEST(x)     ( (ntohl(*(uint32*)(x + 2)) == 0x4E000001 ) && \
			(ntohs(*(uint16*)(x)) == 0x0115 ) )
#define MAC_ADDR_MRP_TOPOLOGY(x) ( (ntohl(*(uint32*)(x + 2)) == 0x4E000002 ) && \
			(ntohs(*(uint16*)(x)) == 0x0115 ) )
#define MAC_ADDR_VTP      MAC_ADDR_CDP
/* All Level 1 Intermediate System Address */
#define MAC_ADDR_ALLLV1IS(x)	( (ntohl(*(uint32*)(x + 2)) == 0xC2000014 ) && \
	(ntohs(*(uint16*)(x)) == 0x0180 ) )
/* All Level 2 Intermediate System Address */
#define MAC_ADDR_ALLLV2IS(x)	( (ntohl(*(uint32*)(x + 2)) == 0xC2000015 ) && \
	(ntohs(*(uint16*)(x)) == 0x0180 ) )
/* All End System Network Entities Address */
#define MAC_ADDR_ALLENDSYSNE(x)	( (ntohl(*(uint32*)(x + 2)) == 0x2B000004 ) && \
	(ntohs(*(uint16*)(x)) == 0x0900 ) )
/* All Intermediate System Network Entities Address */
#define MAC_ADDR_ALLISNE(x)	( (ntohl(*(uint32*)(x + 2)) == 0x2B000005 ) && \
	(ntohs(*(uint16*)(x)) == 0x0900 ) )
#define MAC_ADDR_FWDPROTO(x) (ntohl(*(uint32*)(x))  == 0x0180c200)
#define MAC_ADDR_L2PTUNNEL(x)    ( (ntohl(*(uint32*)(x + 2)) == 0x0CCDCDD0 ) && \
	(ntohs(*(uint16*)(x)) == 0x0100 ) )
#define MAC_ADDR_ERPS(x)	( (ntohl(*(uint32*)(x + 2)) == 0xA7000001 ) && \
	(ntohs(*(uint16*)(x)) == 0x0119 ) )
#define MAC_ADDR_PTP(x)		( (ntohl(*(uint32*)(x + 2)) == 0x19000000 ) && \
		(ntohs(*(uint16*)(x)) == 0x011b ) )
#define MAC_ADDR_STACK_SEND_MSU(x)	( (ntohl(*(uint32*)(x + 2)) == 0xffffffff) && (ntohs(*(uint16*)(x)) == 0x00ff) )

#endif
#define MAC_ADDR_LACP MAC_ADDR_SLOW
#define MAC_ADDR_ISIS(x)	(MAC_ADDR_ALLLV1IS(x) ||	\
	MAC_ADDR_ALLLV2IS(x) || MAC_ADDR_ALLENDSYSNE(x) || MAC_ADDR_ALLISNE(x))
#define MAC_ADDR_EAPS(x)	(MAC_ADDR_BDCOM_UC_1(x) || MAC_ADDR_EAPS_XTREM(x))
#define MAC_ADDR_MRP(x)		(MAC_ADDR_MRP_TEST(x) || MAC_ADDR_MRP_TOPOLOGY(x))

typedef struct EthernetHeader {
	uint8	dmac[L2_MAC_ADDRESS_LEN];			/* 0x06:ETHER_ADDRLEN */
	uint8	smac[L2_MAC_ADDRESS_LEN];
	uint16	type;
} __attribute__ ((packed)) EthHd;

typedef struct VlanHeader {
	uint8   dmac[L2_MAC_ADDRESS_LEN];
	uint8   smac[L2_MAC_ADDRESS_LEN];
	uint16  protocol;
	uint16  flag;
	uint16  type;
} __attribute__ ((packed)) VlanHd;

#endif /* !_NET_ETHERNET_H_ */

