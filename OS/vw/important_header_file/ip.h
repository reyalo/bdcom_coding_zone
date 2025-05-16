/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)ip.h	8.2 (Berkeley) 6/1/94
 * $FreeBSD: src/sys/netinet/ip.h,v 1.16 1999/08/28 00:49:19 peter Exp $
 */

#ifndef _IP_IP_H_
#define _IP_IP_H_

#ifdef __cplusplus
    extern "C" {
#endif

#include <global.h>
#include <ni.h>
#include "ip/in.h"
#include "ip/in_systm.h"
#include "ip/inet.h"
#include "ip/errno.h"

/*
#include "ip/ioccom.h"
#include "ip/systm.h"
*/
/*
 * Definitions for internet protocol version 4.
 * Per RFC 791, September 1981.
 */

#define IPFLOW_CACHE_TYPE_NORMAL		0x0000001
#define IPFLOW_CACHE_TYPE_FSC      		0x0000002

#define	IPVERSION	4

#define IPREG_NAT_SET_CACHE_DELETE_NOTIFY  0x00000001                              /*add by ght, 2k6.03.28*/
#define IP_CACHE_DELETE_NOTIFY                      IPREG_NAT_SET_CACHE_DELETE_NOTIFY  /*add by ght, 2k6.03.28*/
#define IPREG_RT_GET_INTEREST_RT_CALLBACK    0x00000001                               /*add by ght, 2k6.03.28*/
int ip_register(UINT32 module, UINT32 function, UINT32 reserve, void *data);

#if defined(ARMEB)
#define ip_get_unaligned_int32(addr, result32, result_type)\
	switch((addr)%4){\
		case 0:\
			result32 = *(result_type*)addr;\
			break;\
		case 1:\
			result32 = (*(result_type*)((char *)addr-1))<<8 | (*(char*)((char*)addr+3));\
			break;\
		case 2:\
			result32 = (*(result_type*)((char *)addr-2))<<16 | (*(result_type*)((char *)addr+2))>>16;\
			break;\
		case 3:\
			result32 = (*(char*)(addr))<<24 | (*(result_type*)((char*)addr+1)>>8);\
			break;\
	}

#elif defined(ARMEL)
#define ip_get_unaligned_int32(addr, result32, result_type)\
	switch((addr)%4){\
		case 0:\
			result32 = *(result_type*)addr;\
			break;\
		case 1:\
			result32 = (*(result_type*)((char *)addr-1))>>8 | (*(char*)((char*)addr+3))<<24;\
			break;\
		case 2:\
			result32 = (*(result_type*)((char *)addr-2))>>16 | (*(result_type*)((char *)addr+2))<<16;\
			break;\
		case 3:\
			result32 = (*(char*)(addr)) | (*(result_type*)((char*)addr+1)<<8);\
			break;\
	}
#endif

/*
 * Structure of an internet header, naked of options.
 */
struct ip {
#ifdef _IP_VHL
	u_char	ip_vhl;			/* version << 4 | header length >> 2 */
#else
/*��ֲ��XSCALE��,������u_int����λ������˶�������.2008.1.7*/
#if BYTE_ORDER == LITTLE_ENDIAN
	u_char	ip_hl:4,		/* header length */
		ip_v:4;			/* version */
#else
	u_char	ip_v:4,			/* version */
		ip_hl:4;		/* header length */
#endif
#endif /* not _IP_VHL */
	u_char	ip_tos;			/* type of service */
	u_short	ip_len;			/* total length */
	u_short	ip_id;			/* identification */
	u_short	ip_off;			/* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	u_char	ip_ttl;			/* time to live */
	u_char	ip_p;			/* protocol */
	u_short	ip_sum;			/* checksum */
	struct	in_addr ip_src,ip_dst;	/* source and dest address */
} __attribute__ ((packed));
#ifdef _IP_VHL
#define	IP_MAKE_VHL(v, hl)	((v) << 4 | (hl))
#define	IP_VHL_HL(vhl)		((vhl) & 0x0f)
#define	IP_VHL_V(vhl)		((vhl) >> 4)
#define	IP_VHL_BORING		0x45
#endif

#define IP_MAX_TOS		0xff

#define	IP_MAXPACKET	65535		/* maximum packet size */

/*
 * Definitions for IP type of service (ip_tos)
 */
#define	IPTOS_LOWDELAY		0x10
#define	IPTOS_THROUGHPUT	0x08
#define	IPTOS_RELIABILITY	0x04
#define	IPTOS_MINCOST		0x02

/*
 * Definitions for IP precedence (also in ip_tos) (hopefully unused)
 */
#define	IPTOS_PREC_NETCONTROL		0xe0
#define	IPTOS_PREC_INTERNETCONTROL	0xc0
#define	IPTOS_PREC_CRITIC_ECP		0xa0
#define	IPTOS_PREC_FLASHOVERRIDE	0x80
#define	IPTOS_PREC_FLASH		0x60
#define	IPTOS_PREC_IMMEDIATE		0x40
#define	IPTOS_PREC_PRIORITY		0x20
#define	IPTOS_PREC_ROUTINE		0x00

/*
 * Definitions for options.
 */
#define	IPOPT_COPIED(o)		((o)&0x80)
#define	IPOPT_CLASS(o)		((o)&0x60)
#define	IPOPT_NUMBER(o)		((o)&0x1f)

#define	IPOPT_CONTROL		0x00
#define	IPOPT_RESERVED1		0x20
#define	IPOPT_DEBMEAS		0x40
#define	IPOPT_RESERVED2		0x60

#define	IPOPT_EOL		0		/* end of option list */
#define	IPOPT_NOP		1		/* no operation */

#define	IPOPT_RR		7		/* record packet route */
#define	IPOPT_TS		68		/* timestamp */
#define	IPOPT_SECURITY		130		/* provide s,c,h,tcc */
#define	IPOPT_LSRR		131		/* loose source route */
#define	IPOPT_SATID		136		/* satnet id */
#define	IPOPT_SSRR		137		/* strict source route */
#define	IPOPT_RA		148		/* router alert */

/*
 * Offsets to fields in options other than EOL and NOP.
 */
#define	IPOPT_OPTVAL	0		/* option ID */
#define	IPOPT_OLEN		1		/* option length */
#define	IPOPT_OFFSET	2		/* offset within option */
#define	IPOPT_MINOFF	4		/* min value of above */

/*
 * Time stamp option structure.
 */
struct	ip_timestamp {
	u_char	ipt_code;	/* IPOPT_TS */
	u_char	ipt_len;	/* size of structure (variable) */
	u_char	ipt_ptr;	/* index of current entry */
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int	ipt_flg:4,	/* flags, see below */
		ipt_oflw:4;		/* overflow counter */
#else
	u_int	ipt_oflw:4,	/* overflow counter */
		ipt_flg:4;		/* flags, see below */
#endif
	union ipt_timestamp {
		n_long	ipt_time[1];
		struct	ipt_ta {
			struct in_addr ipt_addr;
			n_long ipt_time;
		} __attribute__ ((packed)) ipt_ta[1];
	} ipt_timestamp;
} __attribute__ ((packed));

/*	
 *	sizeof(struct ipt_ta) = 8
 *	sizeof(union ipt_timestamp) = 8
 *	sizeof(struct ip_timestamp) = 12
 */

/* flag bits for ipt_flg */
#define	IPOPT_TS_TSONLY		0		/* timestamps only */
#define	IPOPT_TS_TSANDADDR	1		/* timestamps and addresses */
#define	IPOPT_TS_PRESPEC	3		/* specified modules only */

/* bits for security (not byte swapped) */
#define	IPOPT_SECUR_UNCLASS	0x0000
#define	IPOPT_SECUR_CONFID	0xf135
#define	IPOPT_SECUR_EFTO	0x789a
#define	IPOPT_SECUR_MMMM	0xbc4d
#define	IPOPT_SECUR_RESTR	0xaf13
#define	IPOPT_SECUR_SECRET	0xd788
#define	IPOPT_SECUR_TOPSECRET	0x6bc5

/*
 * Internet implementation parameters.
 */
#define IPMINTTL	0
#define IPMAXTTL	255

/* #define	IPDEFTTL	64		 default ttl, from RFC 1340 */
#define IPDEFTTL	255		/* default ttl, from Cisco, RFC 1340 is 64 */

#define	IPFRAGTTL	60		/* time to live for frags, slowhz */
#define	IPTTLDEC	1		/* subtracted when forwarding */

#define	IP_MSS		576		/* default maximum segment size */

/* param type added by xuhao , delive to ip_input(...) 
 * sorry, i know it's ugly ... : ( 
 */
#define NI_CAST_TYPE_MLOOPBACK      0x4

#ifdef OS_VXWORKS
#define ipv4_id		ipv4_id
#define ip_hashinit	ip_hashinit
#define ipv4_output	ipv4_output
#else
#define ipv4_id		ip_id
#define ip_hashinit	hashinit
#define ipv4_output	ip_output
#endif
extern ushort ipv4_id ;

extern void * ip_hashinit( int elements, unsigned long *hashmask ) ;
int ip_vrf_enable_status(void);/*when vrf enable, return 1 else return 0*/

#ifdef INCLUDE_L2FAKEROUTING
struct l2_add_route
{
	ULONG dest;
	ULONG mask;
	ULONG gateway;
	ULONG hwexist;
	struct l2_add_route* next;
};

#endif

#ifdef __cplusplus
    }
#endif

#endif
