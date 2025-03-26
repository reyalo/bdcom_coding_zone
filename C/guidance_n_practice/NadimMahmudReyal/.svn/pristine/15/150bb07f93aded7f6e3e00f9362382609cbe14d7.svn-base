#include "Pointer & Struct.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <endian.h>


// Convert a hex string to a byte
unsigned char hex_to_byte(const char *hex) {
    unsigned char byte;
    sscanf(hex, "%2hhx", &byte);
    return byte;
}

// Check if a packet exists and increment ref count if found
int find_and_increment(EthernetII_IPv4_Packet *head, unsigned int src_ip, unsigned int dest_ip) {
    while (head != NULL) {
        if (head->ip.src_ip == src_ip && head->ip.dest_ip == dest_ip) {
            head->ref_count++;
            return 1;
        }
        head = head->next;
    }
    return 0;
}

// Parse raw bytes into an EthernetII_IPv4_Packet structure
EthernetII_IPv4_Packet *parse_raw_packet(const uint8_t *raw_packet, size_t length) {
    EthernetII_IPv4_Packet *packet = (EthernetII_IPv4_Packet *)malloc(sizeof(EthernetII_IPv4_Packet));
    if (!packet) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Copy Ethernet II header
    memcpy(&packet->eth, raw_packet, sizeof(EthernetIIHeader));

    // Verify if it is an IPv4 packet
    if (ntohs(packet->eth.ethertype) != 0x0800) {
        printf("Not an IPv4 packet! Ignoring...\n");
        free(packet);
        return NULL;
    }

    // Copy IPv4 header
    memcpy(&packet->ip, raw_packet + sizeof(EthernetIIHeader), sizeof(IPv4Header));
    packet->ref_count = 1;
    packet->next = NULL;

    return packet;
}

// Display the details of a packet
void display_packet_info(const EthernetII_IPv4_Packet *packet) {
    printf("Ethernet II Frame:\n");
    printf("  Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
           packet->eth.dest_mac[0], packet->eth.dest_mac[1], packet->eth.dest_mac[2],
           packet->eth.dest_mac[3], packet->eth.dest_mac[4], packet->eth.dest_mac[5]);

    printf("  Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
           packet->eth.src_mac[0], packet->eth.src_mac[1], packet->eth.src_mac[2],
           packet->eth.src_mac[3], packet->eth.src_mac[4], packet->eth.src_mac[5]);
    
    printf("  EtherType: 0x%04X\n", ntohs(packet->eth.ethertype));

    printf("\nIPv4 Header:\n");
    printf("  Version: %d\n", packet->ip.version);
    printf("  IHL: %d bytes\n", packet->ip.ihl * 4);
    printf("  Type of Service: 0x%02X\n", packet->ip.tos);
    printf("  Total Length: %d bytes\n", ntohs(packet->ip.total_length));
    printf("  Identification: 0x%04X\n", ntohs(packet->ip.identification));
    
    // Display individual flags
    printf("  Flags:\n");
    printf("    Reserved: %d\n", packet->ip.reserved_flag);
    printf("    Don't Fragment: %d\n", packet->ip.df_flag);
    printf("    More Fragments: %d\n", packet->ip.mf_flag);
    printf("  Fragment Offset: %d\n", 8*((packet->ip.frag_offset_high << 8) | packet->ip.frag_offset_low));

    printf("  Time to Live: %d\n", packet->ip.ttl);
    printf("  Protocol: ");
    switch(packet->ip.protocol) {
        case 1:  printf("ICMP (1)\n"); break;
        case 6:  printf("TCP (6)\n"); break;
        case 17: printf("UDP (17)\n"); break;
        default: printf("Other (%d)\n", packet->ip.protocol);
    }
    printf("  Header Checksum: 0x%04X\n", ntohs(packet->ip.header_checksum));

    struct in_addr src, dest;
    src.s_addr = packet->ip.src_ip;
    dest.s_addr = packet->ip.dest_ip;
    printf("  Source IP: %s\n", inet_ntoa(src));
    printf("  Destination IP: %s\n", inet_ntoa(dest));
    printf("\n\n");
}

void display_packet_info_listed(const EthernetII_IPv4_Packet *packet) {
    printf("Ethernet II Frame:\n");
    printf("  Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
           packet->eth.dest_mac[0], packet->eth.dest_mac[1], packet->eth.dest_mac[2],
           packet->eth.dest_mac[3], packet->eth.dest_mac[4], packet->eth.dest_mac[5]);

    printf("  Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
           packet->eth.src_mac[0], packet->eth.src_mac[1], packet->eth.src_mac[2],
           packet->eth.src_mac[3], packet->eth.src_mac[4], packet->eth.src_mac[5]);
    

    printf("IPv4 Header:\n");
    
    struct in_addr src, dest;
    src.s_addr = packet->ip.src_ip;
    dest.s_addr = packet->ip.dest_ip;
    printf("  Source IP: %s\n", inet_ntoa(src));
    printf("  Destination IP: %s\n", inet_ntoa(dest));
    printf("  Ref Count: %d\n", packet->ref_count);
    printf("\n\n");
}

// Process a packet: add to list or increment ref count if duplicate
void process_packet(EthernetII_IPv4_Packet **head, EthernetII_IPv4_Packet *packet, int *total_packets) {
    if (!find_and_increment(*head, packet->ip.src_ip, packet->ip.dest_ip)) {
        packet->next = *head;
        *head = packet;
        (*total_packets)++;
    } else {
        free(packet);
    }
}

// Free the memory allocated for the packet list
void free_packet_list(EthernetII_IPv4_Packet *head) {
    EthernetII_IPv4_Packet *current = head;
    while (current != NULL) {
        EthernetII_IPv4_Packet *temp = current;
        current = current->next;
        free(temp);
    }
}

// Main function to read packets from a file and process them
int main() {
    FILE *input_file = fopen("packets.txt", "r");
    if (!input_file) {
        printf("Error opening files!\n");
        return 1;
    }

    char line[MAX_LINE];
    uint8_t raw_packet[MAX_BYTES];
    int packet_len = 0;
    EthernetII_IPv4_Packet *head = NULL;
    int total_packets = 0;
    int reading_packet = 0;

    // Read each line from the input file
    printf("________________Showing All The Packet_________________\n\n");

    while (fgets(line, sizeof(line), input_file)) {
        if (line[0] == '\n') continue;

        // Start reading new packet when "0000" is encountered
        if (strncmp(line, "0000", 4) == 0) {
            if (packet_len > 0) {
                // Process previous packet if exists
                //show current packet
                EthernetII_IPv4_Packet *packet = (EthernetII_IPv4_Packet *)&raw_packet;
                display_packet_info(packet);
                EthernetII_IPv4_Packet *parsed_packet = parse_raw_packet(raw_packet, packet_len);
                if (parsed_packet) {
                    process_packet(&head, parsed_packet, &total_packets);
                }
            }
            packet_len = 0;
            reading_packet = 1;
        }

        // Read packet data if we're in a packet
        if (reading_packet) {
            char *hex_start = line + 6;  // Skip offset
            char *token = strtok(hex_start, " \n\r");
            
            // Process each hex byte in the line
            while (token != NULL && strlen(token) == 2) {
                raw_packet[packet_len++] = hex_to_byte(token);
                token = strtok(NULL, " \n\r");
            }
        }
    }

    // Process the last packet if exists
    if (packet_len > 0) {
        //show current packet
        EthernetII_IPv4_Packet *packet = (EthernetII_IPv4_Packet *)&raw_packet;
        display_packet_info(packet);

        EthernetII_IPv4_Packet *parsed_packet = parse_raw_packet(raw_packet, packet_len);
        if (parsed_packet) {
            process_packet(&head, parsed_packet, &total_packets);
        }
        packet_len = 0;
    }
    

    fclose(input_file);


    printf("\n\n________________Showing from  the list(No Duplicate Packet)_________________\n\n");

    printf("Total Packets: %d\n\n", total_packets);

    // Display all packets from link list
    EthernetII_IPv4_Packet *current = head;
    while (current != NULL) {
        display_packet_info_listed(current);
        current = current->next;
    }
    
    // Free all allocated memory
    free_packet_list(head);
    return 0;
}