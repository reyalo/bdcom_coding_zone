#include <taskLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <stdlib.h>  /* For rand() */
#include <stdio.h>   /* For snprintf */
#include <libsys/mblk.h>    /* For mblk_t and parsing APIs */

void os_demo_mblk_handle(mblk_t *mb)
{
    mblk_t *mbp = mb;
    unsigned char *ptr;
    long len;

    /* Check if the mblk is valid */
    if (mbp == NULL)
    {
        Print("Received NULL mblk\n");
        return;
    }

    /* Get total length of the packet */
    len = mb_len(mbp);
    Print("Packet length: %ld bytes\n", len);
    if (len < 14) /* Minimum for Ethernet header */
    {
        Print("Packet too short for Ethernet header: %ld bytes\n", len);
        return; /* Do not free here to avoid potential double-free */
    }

    /* Dump raw packet data for debugging (first 64 bytes or less) */
    Print("Dumping raw packet:\n");
    mb_dump("Raw Packet", mbp, 0, len < 64 ? len : 64);

    /* Check EtherType (offset 12) to determine packet type */
    ptr = mbp->b_rptr + 12;
    unsigned short ethertype = (ptr[0] << 8) | ptr[1]; /* Big-endian: MSB first */
    Print("EtherType: 0x%04x\n", ethertype);

    int offset = 14; /* Default offset for Ethernet header */
    if (ethertype == 0x8100) { /* VLAN-tagged frame */
        Print("VLAN-tagged frame detected\n");
        offset += 4; /* Skip 4-byte VLAN tag (2 bytes EtherType + 2 bytes TCI) */
        ptr = mbp->b_rptr + 16; /* Point to inner EtherType (after VLAN tag) */
        ethertype = (ptr[0] << 8) | ptr[1]; /* Big-endian: MSB first */
        Print("Inner EtherType: 0x%04x\n", ethertype);
    }

    if (ethertype == 0x0806) { /* ARP */
        Print("ARP Packet\n");
        ptr = mbp->b_rptr;
        Print("Destination Mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
              ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
        ptr += 6;
        Print("Source Mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
              ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
        return; /* Do not free here */
    }

    if (ethertype != 0x0800) { /* Not an IP packet */
        Print("Not an IP packet\n");
        return; /* Do not free here */
    }

    /* Ensure packet is long enough for IPv4 + ICMP with adjusted offset */
    if (len < offset + 20 + 8) {
        Print("Packet too short for IP + ICMP: %ld bytes\n", len);
        return; /* Do not free here */
    }

    /* Check IPv4 protocol at adjusted offset */
    ptr = mbp->b_rptr + offset + 9; /* IPv4 Protocol field */
    unsigned char protocol = *ptr;
    Print("IPv4 Protocol: %d\n", protocol);
    if (protocol != 1) { /* Not ICMP */
        Print("Unknown protocol (not ICMP)\n");
        return; /* Do not free here */
    }

    /* Determine if this is an Echo or Echo Reply */
    unsigned char icmp_type = 0;
    ptr = mbp->b_rptr + offset + 20; /* Skip to ICMP header */
    icmp_type = *ptr; /* ICMP Type */
    Print(icmp_type == 8 ? "Echo\n\n" : "Echo Reply:\n\n");

    /* Parse Ethernet Header (offset bytes) */
    Print("Ethernet Packet\n");
    ptr = mbp->b_rptr;
    Print("Destination Mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
          ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
    ptr += 6;
    Print("Source Mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
          ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
    ptr += 6;

    /* Parse IPv4 Header (20 bytes) */
    Print("IP Packet\n");
    ptr = mbp->b_rptr + offset;
    unsigned char version_ihl = *ptr++;
    unsigned char version = (version_ihl >> 4) & 0xF;
    unsigned char ihl = (version_ihl & 0xF) * 4;
    Print("Header Length: %d\n", ihl);
    Print("Header Version: %d\n", version);
    unsigned char tos = *ptr++;
    Print("TOS: %d\n", tos);
    unsigned short total_length = (ptr[0] << 8) | ptr[1]; ptr += 2; /* Big-endian */
    Print("Total Length: %d\n", total_length);
    unsigned short id = (ptr[0] << 8) | ptr[1]; ptr += 2; /* Big-endian */
    Print("Header ID: %d\n", id);
    unsigned short flags_offset = (ptr[0] << 8) | ptr[1]; ptr += 2; /* Big-endian */
    unsigned short fragment_offset = flags_offset & 0x1FFF;
    Print("Fragment Offset: %d\n", fragment_offset);
    unsigned char ttl = *ptr++;
    Print("TTL: %d\n", ttl);
    protocol = *ptr++;
    Print(protocol == 1 ? "Protocol: ICMP\n" : "Protocol: %d\n", protocol);
    unsigned short checksum = (ptr[0] << 8) | ptr[1]; ptr += 2; /* Big-endian */
    Print("Header Checksum: %d\n", checksum);
    unsigned char src_ip[4], dst_ip[4];
    src_ip[0] = *ptr++; src_ip[1] = *ptr++; src_ip[2] = *ptr++; src_ip[3] = *ptr++;
    Print("Source IP: %d.%d.%d.%d\n", src_ip[0], src_ip[1], src_ip[2], src_ip[3]);
    dst_ip[0] = *ptr++; dst_ip[1] = *ptr++; dst_ip[2] = *ptr++; dst_ip[3] = *ptr++;
    Print("Destination IP: %d.%d.%d.%d\n", dst_ip[0], dst_ip[1], dst_ip[2], dst_ip[3]);

    /* Parse ICMP Header (8 bytes) */
    Print("ICMP Packet\n");
    ptr = mbp->b_rptr + offset + 20;
    Print(icmp_type == 8 ? "Type: Echo " : "Type: Echo Reply ");
    unsigned char code = *ptr++;
    Print("Code: %d\n", code);
    unsigned short icmp_checksum = (ptr[0] << 8) | ptr[1]; ptr += 2; /* Big-endian */
    Print("Checksum: %d\n", icmp_checksum);
    unsigned short icmp_id = (ptr[0] << 8) | ptr[1]; ptr += 2; /* Big-endian */
    Print("ID: %d\n", icmp_id);
    unsigned short sequence = (ptr[0] << 8) | ptr[1]; ptr += 2; /* Big-endian */
    Print("Sequence: %d\n", sequence);

    /* Do not free the mblk here to avoid double-free */
}

void os_demo_init()
{
    Print("________________________________REYAL_____________________________(0)\n");
}