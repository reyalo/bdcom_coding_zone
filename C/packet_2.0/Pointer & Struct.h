#ifndef POINTER_AND_STRUCT_H
#define POINTER_AND_STRUCT_H

#include <stdint.h>
#include <stddef.h>

#define MAX_LINE 256
#define MAX_BYTES 1500

typedef struct {
    uint8_t  dest_mac[6];
    uint8_t  src_mac[6];
    uint16_t ethertype;
} __attribute__((packed)) EthernetIIHeader;

typedef struct {
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        uint8_t  ihl:4;
        uint8_t  version:4;
    #elif __BYTE_ORDER == __BIG_ENDIAN
        uint8_t  version:4;
        uint8_t  ihl:4;
    #endif
    uint8_t  tos;
    uint16_t total_length;
    uint16_t identification;
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        uint16_t frag_offset_high:5;
        uint16_t mf_flag:1;
        uint16_t df_flag:1;
        uint16_t reserved_flag:1;
        uint16_t frag_offset_low:8;
    #elif __BYTE_ORDER == __BIG_ENDIAN
        uint16_t reserved_flag:1;
        uint16_t df_flag:1;
        uint16_t mf_flag:1;
        uint16_t frag_offset_high:5;
        uint16_t frag_offset_low:8;
    #endif
    uint8_t  ttl;
    uint8_t  protocol;
    uint16_t header_checksum;
    uint32_t src_ip;
    uint32_t dest_ip;
} __attribute__((packed)) IPv4Header;

typedef struct Packet {
    EthernetIIHeader eth;
    IPv4Header ip;
    int ref_count;
    struct Packet *next;
} __attribute__((packed)) EthernetII_IPv4_Packet;

unsigned char hex_to_byte(const char *hex);
int find_and_increment(EthernetII_IPv4_Packet *head, unsigned int src_ip, unsigned int dest_ip);
EthernetII_IPv4_Packet *parse_raw_packet(const uint8_t *raw_packet, size_t length);
void display_packet_info(const EthernetII_IPv4_Packet *packet);
void display_packet_info_listed(const EthernetII_IPv4_Packet *packet);
void process_packet(EthernetII_IPv4_Packet **head, EthernetII_IPv4_Packet *packet, int *total_packets);
void free_packet_list(EthernetII_IPv4_Packet *head);

#endif