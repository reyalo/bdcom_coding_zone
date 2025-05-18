#include <taskLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <stdlib.h>
#include <stdio.h>
#include <types.h>
#include <netinet/in.h>

/* Include mblk.h last to avoid header conflicts */
#include <libsys/mblk.h>

/* Constants for ICMP types */
#define ICMP_ECHO_REQUEST  8
#define ICMP_ECHO_REPLY    0

#define ETHERTYPE_VLAN    0x8100  /* IEEE 802.1Q VLAN tagging */
#define ETHERTYPE_IP      0x0800  /* IP protocol */

typedef struct {
    UINT8  dest_mac[6];
    UINT8  src_mac[6];
    UINT16 ethertype;
} __attribute__((packed)) EthernetHeader;

typedef struct {
    UINT16 tci;           /* Priority(3) + CFI(1) + VLAN ID(12) */
    UINT16 encap_proto;   /* Encapsulated protocol */
} __attribute__((packed)) VLANHeader;

typedef struct {
    UINT8  version_ihl;
    UINT8  tos;
    UINT16 total_length;
    UINT16 identification;
    UINT16 flags_frag_offset;
    UINT8  ttl;
    UINT8  protocol;
    UINT16 header_checksum;
    UINT32 src_ip;    // Changed from UINT8[4] to UINT32
    UINT32 dest_ip;   // Changed from UINT8[4] to UINT32
} __attribute__((packed)) IPv4Header;

typedef struct {
    UINT8  type;
    UINT8  code;
    UINT16 checksum;
    UINT16 identifier;
    UINT16 sequence;
} __attribute__((packed)) ICMPHeader;

void os_demo_mblk_handle(mblk_t *mb)
{
    if (!mb || mb_len(mb) < sizeof(EthernetHeader)) {
        Print("Invalid packet\n");
        return;
    }

    EthernetHeader *eth = (EthernetHeader *)mb->b_rptr;
    unsigned char *next_header = mb->b_rptr + sizeof(EthernetHeader);
    UINT16 ether_type = eth->ethertype; // No ntohs since big-endian

    // Handle VLAN tag if present
    if (ether_type == ETHERTYPE_VLAN) {
        VLANHeader *vlan = (VLANHeader *)next_header;
        ether_type = vlan->encap_proto;  // No ntohs since big-endian
        next_header += sizeof(VLANHeader);
    }

    // Validate packet length for IPv4
    if (mb_len(mb) < (next_header - mb->b_rptr) + sizeof(IPv4Header)) {
        Print("Packet too short for IPv4 header\n");
        return;
    }

    // Print Ethernet information
    Print("Ethernet Packet\n");
    Print("      Destination Mac:%02x:%02x:%02x:%02x:%02x:%02x\n",
          eth->dest_mac[0], eth->dest_mac[1], eth->dest_mac[2],
          eth->dest_mac[3], eth->dest_mac[4], eth->dest_mac[5]);
    Print("      Source Mac:%02x:%02x:%02x:%02x:%02x:%02x\n",
          eth->src_mac[0], eth->src_mac[1], eth->src_mac[2],
          eth->src_mac[3], eth->src_mac[4], eth->src_mac[5]);

    // Only process IP packets
    if (ether_type != ETHERTYPE_IP) {
        Print("Not an IP packet (ethertype: 0x%04x)\n", ether_type);
        return;
    }

    // Get ICMP type
    IPv4Header *ip = (IPv4Header *)next_header;
    int ip_header_len = (ip->version_ihl & 0x0F) * 4;
    unsigned char icmp_type = *(next_header + ip_header_len);
    if (icmp_type == ICMP_ECHO_REQUEST) {
        Print("Echo\n\n");
    } else {
        Print("Echo Reply:\n");
    }

    // Validate IPv4 header length
    if (ip_header_len < 20 || mb_len(mb) < (next_header - mb->b_rptr) + ip_header_len + sizeof(ICMPHeader)) {
        Print("Invalid IPv4 header length or packet too short for ICMP\n");
        return;
    }

    // Print IP information
    Print("IP Packet\n");
    Print("      Header Length:%d\n", ip_header_len);
    Print("      Header Version:%d\n", (ip->version_ihl >> 4) & 0x0F);
    Print("      TOS:%d\n", ip->tos);
    Print("      Total Length:%d\n", ip->total_length);
    Print("      Header ID: %d\n", ip->identification);
    Print("      offset:%d\n", ip->flags_frag_offset & 0x1FFF);
    Print("      TTL:%d\n", ip->ttl);
    Print("      Protocol: ICMP\n");
    Print("      Header Checksum: %d\n", ip->header_checksum);

    struct in_addr src_addr, dst_addr;
    src_addr.s_addr = ip->src_ip;
    dst_addr.s_addr = ip->dest_ip;
    Print("      Source IP: %s\n", inet_ntoa(src_addr));
    Print("      Destination IP: %s\n", inet_ntoa(dst_addr));
    
    // Print ICMP information
    ICMPHeader *icmp = (ICMPHeader *)(next_header + ip_header_len);
    Print("ICMP Packet\n");
    Print("      Type: %s\n Code: %d\n",
          icmp->type == ICMP_ECHO_REQUEST ? "Echo" : "Echo Reply", icmp->code);
    Print("      Checksum:%d\n", icmp->checksum);
    Print("      ID:%d\n", icmp->identifier);
    Print("      Sequence: %d\n", icmp->sequence);
}

void os_demo_init()
{
    Print("________________________________REYAL(7)_____________________________(0)\n");
}