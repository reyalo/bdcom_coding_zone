#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  // For inet_pton and inet_ntop

#define MAX_LINE_LENGTH 256

// Define a structure for packet data
typedef struct Packet {
    char src_mac[18];
    char dst_mac[18];
    unsigned int src_ip;
    unsigned int dst_ip;
    int ref_count;
    struct Packet *next;
} Packet;

// Function to convert raw bytes to MAC address string
void bytes_to_mac(const unsigned char *bytes, char *mac_str) {
    sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x",
            bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]);
}

// Function to convert raw bytes to IP address string
void bytes_to_ip(const unsigned char *bytes, char *ip_str) {
    sprintf(ip_str, "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2], bytes[3]);
}

// Function to create a new packet node
Packet* create_packet(const char *src_mac, const char *dst_mac, const char *src_ip_str, const char *dst_ip_str) {
    Packet *new_packet = (Packet *)malloc(sizeof(Packet));
    if (!new_packet) {
        perror("Failed to allocate memory for new packet");
        exit(EXIT_FAILURE);
    }
    strncpy(new_packet->src_mac, src_mac, sizeof(new_packet->src_mac) - 1);
    new_packet->src_mac[sizeof(new_packet->src_mac) - 1] = '\0';
    strncpy(new_packet->dst_mac, dst_mac, sizeof(new_packet->dst_mac) - 1);
    new_packet->dst_mac[sizeof(new_packet->dst_mac) - 1] = '\0';

    if (inet_pton(AF_INET, src_ip_str, &(new_packet->src_ip)) != 1) {
        fprintf(stderr, "Failed to convert source IP address: %s\n", src_ip_str);
        free(new_packet);
        exit(EXIT_FAILURE);
    }

    if (inet_pton(AF_INET, dst_ip_str, &(new_packet->dst_ip)) != 1) {
        fprintf(stderr, "Failed to convert destination IP address: %s\n", dst_ip_str);
        free(new_packet);
        exit(EXIT_FAILURE);
    }

    new_packet->ref_count = 1;
    new_packet->next = NULL;
    return new_packet;
}

// Function to append a packet to the linked list or increment the reference counter if it already exists
void append_or_increment_packet(Packet **head, Packet *new_packet) {
    Packet *current = *head;
    while (current != NULL) {
        if (current->src_ip == new_packet->src_ip && current->dst_ip == new_packet->dst_ip) {
            current->ref_count++;
            free(new_packet);  // Free the new packet as it's not needed
            return;
        }
        current = current->next;
    }
    // If the packet does not exist, append it to the linked list
    new_packet->next = *head;
    *head = new_packet;
}

// Function to process packets from the input file and build the linked list
void process_packet(FILE *input_file, Packet **head) {
    unsigned char buffer[1500];  // Buffer to hold raw packet data
    char src_mac[18], dst_mac[18], src_ip_str[16], dst_ip_str[16];
    char line[MAX_LINE_LENGTH];
    int byte_count = 0;
    unsigned char packet[1500];
    int packet_len = 0;

    while (fgets(line, sizeof(line), input_file)) {
        if (line[0] == '\n' || line[0] == '\r') {
            // Process the packet
            if (packet_len > 0) {
                // Extract MAC addresses
                bytes_to_mac(packet + 6, src_mac);  // Source MAC address starts at byte 6
                bytes_to_mac(packet, dst_mac);      // Destination MAC address starts at byte 0

                // Extract IP addresses
                bytes_to_ip(packet + 26, src_ip_str);  // Source IP address starts at byte 26
                bytes_to_ip(packet + 30, dst_ip_str);  // Destination IP address starts at byte 30

                // Create a new packet and append it to the linked list or increment the reference counter
                Packet *new_packet = create_packet(src_mac, dst_mac, src_ip_str, dst_ip_str);
                append_or_increment_packet(head, new_packet);

                // Reset packet length for the next packet
                packet_len = 0;
            }
        } else {
            // Read the packet data
            char *token = strtok(line, " ");
            while (token != NULL) {
                if (byte_count >= 4) {  // Skip the first 4 bytes (offset)
                    packet[packet_len++] = (unsigned char)strtol(token, NULL, 16);
                }
                token = strtok(NULL, " ");
                byte_count++;
            }
            byte_count = 0;
        }
    }

    // Process the last packet if the file does not end with a blank line
    if (packet_len > 0) {
        // Extract MAC addresses
        bytes_to_mac(packet + 6, src_mac);  // Source MAC address starts at byte 6
        bytes_to_mac(packet, dst_mac);      // Destination MAC address starts at byte 0

        // Extract IP addresses
        bytes_to_ip(packet + 26, src_ip_str);  // Source IP address starts at byte 26
        bytes_to_ip(packet + 30, dst_ip_str);  // Destination IP address starts at byte 30

        // Create a new packet and append it to the linked list or increment the reference counter
        Packet *new_packet = create_packet(src_mac, dst_mac, src_ip_str, dst_ip_str);
        append_or_increment_packet(head, new_packet);
    }
}

// Function to write the linked list to the output file
void write_packets_to_file(FILE *output_file, Packet *head) {
    Packet *current = head;
    char src_ip_str[INET_ADDRSTRLEN], dst_ip_str[INET_ADDRSTRLEN];
    while (current != NULL) {
        inet_ntop(AF_INET, &(current->src_ip), src_ip_str, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(current->dst_ip), dst_ip_str, INET_ADDRSTRLEN);
        fprintf(output_file, "SMAC: %s | DMAC: %s | SIP: %s | DIP: %s | Ref Count: %d\n",
                current->src_mac, current->dst_mac, src_ip_str, dst_ip_str, current->ref_count);
        current = current->next;
    }
}

// Function to free the linked list
void free_packets(Packet *head) {
    Packet *current = head;
    while (current != NULL) {
        Packet *next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    FILE *input_file = fopen("packets_bkp.txt", "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output_file = fopen("packets_output.txt", "w");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }

    Packet *head = NULL;
    process_packet(input_file, &head);
    write_packets_to_file(output_file, head);

    fclose(input_file);
    fclose(output_file);
    free_packets(head);

    printf("Packets saved to packets_output.txt\n");
    return 0;
}