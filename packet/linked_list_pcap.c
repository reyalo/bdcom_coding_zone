#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <string.h>

// Structure for linked list node
typedef struct PacketNode {
    struct pcap_pkthdr header; // Packet header
    unsigned char *data;       // Packet data
    struct PacketNode *next;   // Pointer to next node
} PacketNode;

// Function to insert a packet into the linked list
void insertPacket(PacketNode **head, struct pcap_pkthdr *header, const unsigned char *packet) {
    PacketNode *newNode = (PacketNode *)malloc(sizeof(PacketNode));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Copy packet header
    newNode->header = *header;
    
    // Allocate memory for packet data
    newNode->data = (unsigned char *)malloc(header->caplen);
    if (!newNode->data) {
        perror("Memory allocation for packet data failed");
        free(newNode);
        exit(EXIT_FAILURE);
    }

    // Copy packet data
    memcpy(newNode->data, packet, header->caplen);
    newNode->next = *head;
    *head = newNode;
}

// Function to print stored packets
void printPackets(PacketNode *head) {
    PacketNode *temp = head;
    int count = 1;
    while (temp) {
        printf("Packet %d: Length = %d bytes\n", count++, temp->header.len);
        temp = temp->next;
    }
}

// Function to free linked list
void freePackets(PacketNode *head) {
    PacketNode *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];  // Error buffer
    pcap_t *handle;                 // File handler
    struct pcap_pkthdr *header;      // Packet header
    const unsigned char *packet;     // Packet data
    PacketNode *head = NULL;         // Head of linked list

    // Open the pcap file
    handle = pcap_open_offline("captured_packets.pcap", errbuf);
    if (!handle) {
        fprintf(stderr, "Error opening pcap file: %s\n", errbuf);
        return EXIT_FAILURE;
    }

    // Read packets from the file
    while (pcap_next_ex(handle, &header, &packet) == 1) {
        insertPacket(&head, header, packet);
    }

    // Close the file
    pcap_close(handle);

    // Print stored packets
    printPackets(head);

    // Free the linked list memory
    freePackets(head);

    return 0;
}
