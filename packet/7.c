#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>      // For memcpy()
#include <arpa/inet.h>   // For ntohs(), ntohl(), inet_ntoa()

#define MAX_LINE 256
#define MAX_BYTES 34

#pragma pack(push, 1) // Ensure no structure padding

// Ethernet II Frame Header
typedef struct {
    uint8_t  dest_mac[6];  // Destination MAC Address
    uint8_t  src_mac[6];   // Source MAC Address
    uint16_t ethertype;    // EtherType (IPv4: 0x0800)
} EthernetIIHeader;

// IPv4 Header (RFC 791)
typedef struct {
    uint8_t  ihl:4;        // Internet Header Length (4 bits)
    uint8_t  version:4;    // IPv4 Version (4 bits)
    uint8_t  tos;          // Type of Service
    uint16_t total_length; // Total Length (bytes)
    uint16_t identification;
    uint16_t flags_frag_offset;
    uint8_t  ttl;          // Time To Live
    uint8_t  protocol;     // Protocol (TCP=6, UDP=17, ICMP=1)
    uint16_t header_checksum;
    uint32_t src_ip;       // Source IP Address
    uint32_t dest_ip;      // Destination IP Address
} IPv4Header;

// Complete Ethernet II + IPv4 Packet Structure
typedef struct Packet{
    EthernetIIHeader eth;
    IPv4Header ip;
    int ref_count;
    struct Packet *next;
} EthernetII_IPv4_Packet;

#pragma pack(pop) // Restore default alignment






// Function to convert hex string to byte
unsigned char hex_to_byte(const char *hex) {
    unsigned char byte;
    sscanf(hex, "%2hhx", &byte);
    return byte;
}

// Function to check if packet exists and increment ref count if found
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
// Function to parse raw bytes into EthernetII_IPv4_Packet
EthernetII_IPv4_Packet *parse_raw_packet(const uint8_t *raw_data, size_t length) {
    // EthernetII_IPv4_Packet packet;

    EthernetII_IPv4_Packet *packet = (EthernetII_IPv4_Packet *)malloc(sizeof(EthernetII_IPv4_Packet));
    if (!packet) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Copy Ethernet II header
    memcpy(&packet->eth, raw_data, sizeof(EthernetIIHeader));

    // Verify if it is an IPv4 packet
    if (ntohs(packet->eth.ethertype) != 0x0800) {
        printf("Not an IPv4 packet! Ignoring...\n");
        return packet;
    }

    // Copy IPv4 header
    memcpy(&packet->ip, raw_data + sizeof(EthernetIIHeader), sizeof(IPv4Header));

    packet->ref_count = 1;
    packet->next = NULL;

    return packet;
}

// Function to display extracted packet details
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
    printf("  Version: %u\n", packet->ip.version);
    printf("  IHL: %u (x4 = %u bytes)\n", packet->ip.ihl, packet->ip.ihl * 4);
    printf("  Total Length: %u\n", ntohs(packet->ip.total_length));
    printf("  TTL: %u\n", packet->ip.ttl);
    printf("  Protocol: %u\n", packet->ip.protocol);

    struct in_addr src, dest;
    src.s_addr = packet->ip.src_ip;
    dest.s_addr = packet->ip.dest_ip;
    printf("  Source IP: %s\n", inet_ntoa(src));
    printf("  Destination IP: %s\n", inet_ntoa(dest));
    printf("  Ref Count: %d\n", packet->ref_count);
    printf("\n\n");
}


void process_packet(EthernetII_IPv4_Packet **head, EthernetII_IPv4_Packet *packet, int *total_packets) {
    
    // check if already exist or not

    // return;

    

    if (!find_and_increment(*head, packet->ip.src_ip, packet->ip.dest_ip)) {
        packet->next = *head;
        *head = packet;
        (*total_packets)++;
    }
    else{
        free(packet);
    }
}




// Example main function to test parsing
int main() {
    



    //********************************************** */

    FILE *input_file = fopen("packets.txt", "r");
    if (!input_file) {
        printf("Error opening files!\n");
        return 1;
    }

    char line[MAX_LINE];
    uint8_t packet[MAX_BYTES];
    int packet_len = 0;
    // Packet *head = NULL;
    EthernetII_IPv4_Packet *head;
    int total_packets = 0;
    int is_new_packet = 0;

    while (fgets(line, sizeof(line), input_file)) {
        if(strlen(line) <= 1){
            continue;
        }

        // for each packet, only first 3 lines are useful for extract MAC and IP
        if(strncmp(line, "0000", 4) == 0 || strncmp(line, "0010", 4) == 0 || strncmp(line, "0020", 4) == 0){
            char *hex_start = line + 6; // Skip offset
            char *token = strtok(hex_start, " ");
            
            // Process each hex byte in the line
            while (token != NULL && strlen(token) == 2 && packet_len < MAX_BYTES) {
                packet[packet_len++] = hex_to_byte(token);
                token = strtok(NULL, " ");
            }

            if(packet_len == MAX_BYTES){
                // now time to extract
                EthernetII_IPv4_Packet *parsed_packet = parse_raw_packet(packet, MAX_BYTES);
                process_packet(&head, parsed_packet, &total_packets);

                // display_packet_info(parsed_packet);
                
                packet_len = 0;
            }


            
        }

    
    
    }
    fclose(input_file);

    //********************************************************************** */


    // Print all packets to console
    EthernetII_IPv4_Packet *current = head;
    while (current != NULL) {
        display_packet_info(current);
        current = current->next;
    }
    
    // free_list(head);



    // Parse the raw packet
    // EthernetII_IPv4_Packet parsed_packet = parse_raw_packet(raw_packet, sizeof(raw_packet));

    // Display extracted information
    // display_packet_info(&parsed_packet);

    return 0;
}


/*
// Example raw packet (Ethernet II + IPv4)
    uint8_t raw_packet[] = {
        0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E, // Destination MAC
        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, // Source MAC
        0x08, 0x00,                         // EtherType: IPv4 (0x0800)

        0x45, 0x00, 0x00, 0x3C,             // IPv4: Version, IHL, TOS, Total Length (0x003C = 60 bytes)
        0x12, 0x34, 0x00, 0x00,             // Identification, Flags/Fragment Offset
        0x40, 0x06, 0x00, 0x00,             // TTL (64), Protocol (TCP=6), Checksum
        0xC0, 0xA8, 0x01, 0x01,             // Source IP: 192.168.1.1
        0xC0, 0xA8, 0x01, 0x02              // Destination IP: 192.168.1.2
    };

*/