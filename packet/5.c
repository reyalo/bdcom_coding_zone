#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_LINE 256
#define MAX_BYTES 1500

// Define packet structure for linked list
typedef struct Packet {
    unsigned char src_mac[6];
    unsigned char dst_mac[6];
    unsigned int src_ip;
    unsigned int dst_ip;
    int ref_count;
    struct Packet *next;
} Packet;

// Function to convert hex string to byte
unsigned char hex_to_byte(const char *hex) {
    unsigned char byte;
    sscanf(hex, "%2hhx", &byte);
    return byte;
}

// Function to create new packet node
Packet* create_packet(unsigned char *packet) {
    Packet *new_packet = (Packet *)malloc(sizeof(Packet));
    if (!new_packet) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Copy MAC addresses
    memcpy(new_packet->src_mac, packet + 6, 6);
    memcpy(new_packet->dst_mac, packet, 6);
    
    // Copy IP addresses as unsigned int
    unsigned char *src_ip = packet + 26;
    unsigned char *dst_ip = packet + 30;
    
    new_packet->src_ip = (src_ip[0] << 24) | (src_ip[1] << 16) | 
                        (src_ip[2] << 8) | src_ip[3];
    new_packet->dst_ip = (dst_ip[0] << 24) | (dst_ip[1] << 16) | 
                        (dst_ip[2] << 8) | dst_ip[3];
    
    new_packet->ref_count = 1;
    new_packet->next = NULL;
    return new_packet;
}

// Function to check if packet exists and increment ref count if found
int find_and_increment(Packet *head, unsigned int src_ip, unsigned int dst_ip) {
    while (head != NULL) {
        if (head->src_ip == src_ip && head->dst_ip == dst_ip) {
            head->ref_count++;
            return 1;
        }
        head = head->next;
    }
    return 0;
}

// Function to write packet info to file
void write_packet_info(FILE *output_file, Packet *packet) {
    fprintf(output_file, "SMAC: %02x:%02x:%02x:%02x:%02x:%02x | ",
            packet->src_mac[0], packet->src_mac[1], packet->src_mac[2],
            packet->src_mac[3], packet->src_mac[4], packet->src_mac[5]);
    
    fprintf(output_file, "DMAC: %02x:%02x:%02x:%02x:%02x:%02x | ",
            packet->dst_mac[0], packet->dst_mac[1], packet->dst_mac[2],
            packet->dst_mac[3], packet->dst_mac[4], packet->dst_mac[5]);
    
    // Print IP addresses from unsigned int
    fprintf(output_file, "SIP: %d.%d.%d.%d | ",
            (packet->src_ip >> 24) & 0xFF,
            (packet->src_ip >> 16) & 0xFF,
            (packet->src_ip >> 8) & 0xFF,
            packet->src_ip & 0xFF);
    
    fprintf(output_file, "DIP: %d.%d.%d.%d | ",
            (packet->dst_ip >> 24) & 0xFF,
            (packet->dst_ip >> 16) & 0xFF,
            (packet->dst_ip >> 8) & 0xFF,
            packet->dst_ip & 0xFF);
    
    fprintf(output_file, "Ref Count: %d\n", packet->ref_count);
}

// Function to free linked list
void free_list(Packet *head) {
    Packet *current = head;
    while (current != NULL) {
        Packet *temp = current;
        current = current->next;
        free(temp);
    }
}

// Function to process a packet
void process_packet(Packet **head, unsigned char *packet, int *total_packets) {
    unsigned int src_ip = (packet[26] << 24) | (packet[27] << 16) | 
                          (packet[28] << 8) | packet[29];
    unsigned int dst_ip = (packet[30] << 24) | (packet[31] << 16) | 
                          (packet[32] << 8) | packet[33];

    if (!find_and_increment(*head, src_ip, dst_ip)) {
        Packet *new_packet = create_packet(packet);
        new_packet->next = *head;
        *head = new_packet;
        (*total_packets)++;
    }
}


int main() {
    FILE *input_file = fopen("packets_bkp.txt", "r");
    FILE *output_file = fopen("packet_info.txt", "w");
    if (!input_file || !output_file) {
        printf("Error opening files!\n");
        return 1;
    }

    char line[MAX_LINE];
    unsigned char packet[MAX_BYTES];
    int packet_len = 0;
    Packet *head = NULL;
    int total_packets = 0;
    int is_new_packet = 0;

    while (fgets(line, sizeof(line), input_file)) {


        // Skip empty lines and process completed packets
        if (strlen(line) <= 1) {
            if (packet_len >= 34) {
                process_packet(&head, packet, &total_packets);
            }
            packet_len = 0;
            is_new_packet = 1;
            continue;
        }

        // Remove ASCII part
        char *ascii_part = strstr(line, "  ");
        if (ascii_part) *ascii_part = '\0';

        // Process packet data
        if (strncmp(line, "0000", 4) == 0) {
            // If we find a new packet start and have data, process the previous packet
            if (!is_new_packet && packet_len >= 34) {
                process_packet(&head, packet, &total_packets);
                packet_len = 0;
            }
            is_new_packet = 0;
        }

        if (strncmp(line, "0000", 4) == 0 || strncmp(line, "00", 2) == 0) {
            char *hex_start = line + 6; // Skip offset
            char *token = strtok(hex_start, " ");
            
            // Process each hex byte in the line
            while (token != NULL && strlen(token) == 2) {
                packet[packet_len++] = hex_to_byte(token);
                token = strtok(NULL, " ");
            }
        }
    }

    // Process last packet if exists
    if (packet_len >= 34) {
        process_packet(&head, packet, &total_packets);
    }

    // Write all packets to output file
    Packet *current = head;
    while (current != NULL) {
        write_packet_info(output_file, current);
        current = current->next;
    }

    fprintf(output_file, "\nTotal packets processed: %d\n", total_packets);

    free_list(head);
    fclose(input_file);
    fclose(output_file);
    printf("Packet information has been saved to packet_info.txt\n");
    return 0;
}
