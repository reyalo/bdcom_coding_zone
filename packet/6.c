#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_LINE 256
#define MAX_BYTES 100

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
    
    new_packet->src_ip =    (packet[26] << 24) | (packet[27] << 16) | 
                            (packet[28] << 8) | packet[29];
    new_packet->dst_ip =    (packet[30] << 24) | (packet[31] << 16) | 
                            (packet[32] << 8) | packet[33];
    
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

void print_console(Packet *packet) {
    printf("SMAC: %02x:%02x:%02x:%02x:%02x:%02x | ",
            packet->src_mac[0], packet->src_mac[1], packet->src_mac[2],
            packet->src_mac[3], packet->src_mac[4], packet->src_mac[5]);
    
    printf("DMAC: %02x:%02x:%02x:%02x:%02x:%02x | ",
            packet->dst_mac[0], packet->dst_mac[1], packet->dst_mac[2],
            packet->dst_mac[3], packet->dst_mac[4], packet->dst_mac[5]);
    
    // Print IP addresses from unsigned int

    
    printf("SIP: %s | ", inet_ntoa((struct in_addr){htonl(packet->src_ip)}));
    printf("DIP: %s | ", inet_ntoa((struct in_addr){htonl(packet->dst_ip)}));

    
    printf("Ref Count: %d\n", packet->ref_count);
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
    FILE *input_file = fopen("packets.txt", "r");
    if (!input_file) {
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
        if(strlen(line) <= 1){
            continue;
        }

        // for each packet, only first 3 lines are useful for extract MAC and IP
        if(strncmp(line, "0000", 4) == 0 || strncmp(line, "0010", 4) == 0 || strncmp(line, "0020", 4) == 0){
            char *hex_start = line + 6; // Skip offset
            char *token = strtok(hex_start, " ");
            
            // Process each hex byte in the line
            while (token != NULL && strlen(token) == 2) {
                packet[packet_len++] = hex_to_byte(token);
                token = strtok(NULL, " ");
            }


            if(strncmp(line, "0020", 4) == 0){
            
                // now time to extract
                process_packet(&head, packet, &total_packets);
                
                packet_len = 0;
            }
        }  
    }


    
    // Print all packets to console
    Packet *current = head;
    while (current != NULL) {
        print_console(current);
        current = current->next;
    }
    
    free_list(head);
    fclose(input_file);
    return 0;
}



/*
    // Function to write packet info to file
    void write_packet_info(FILE *output_file, Packet *packet) {
    fprintf(output_file, "SMAC: %02x:%02x:%02x:%02x:%02x:%02x | ",
            packet->src_mac[0], packet->src_mac[1], packet->src_mac[2],
            packet->src_mac[3], packet->src_mac[4], packet->src_mac[5]);
    
    fprintf(output_file, "DMAC: %02x:%02x:%02x:%02x:%02x:%02x | ",
            packet->dst_mac[0], packet->dst_mac[1], packet->dst_mac[2],
            packet->dst_mac[3], packet->dst_mac[4], packet->dst_mac[5]);
    
    // Print IP addresses from unsigned int

    
    fprintf(output_file, "SIP: %s | ", inet_ntoa((struct in_addr){htonl(packet->src_ip)}));
    fprintf(output_file, "DIP: %s | ", inet_ntoa((struct in_addr){htonl(packet->dst_ip)}));

    
    fprintf(output_file, "Ref Count: %d\n", packet->ref_count);
    }




    FILE *output_file = fopen("packet_info.txt", "w");
    if (!output_file) {
        printf("Error opening output file!\n");
        return 1;
    }
    // Write all packets to output file
    Packet *current = head;
    while (current != NULL) {
        write_packet_info(output_file, current);
        current = current->next;
    }
    printf("Packet information has been saved to packet_info.txt\n");
    fclose(output_file);

*/