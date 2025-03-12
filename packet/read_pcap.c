#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>

// Define a linked list node
struct PacketNode {
    struct pcap_pkthdr header;
    unsigned char *data;
    struct PacketNode *next;
};

// Function to add a packet to the linked list
void add_packet(struct PacketNode **head, const struct pcap_pkthdr *pkt_header, const unsigned char *pkt_data) {
    struct PacketNode *new_node = (struct PacketNode *)malloc(sizeof(struct PacketNode));
    new_node->header = *pkt_header;
    new_node->data = (unsigned char *)malloc(pkt_header->caplen);
    memcpy(new_node->data, pkt_data, pkt_header->caplen);
    new_node->next = *head;
    *head = new_node;
}

// Function to print packet info
void print_packets(struct PacketNode *head) {
    struct PacketNode *current = head;
    int count = 1;
    while (current) {
        printf("Packet %d: Length=%d bytes\n", count, current->header.len);
        current = current->next;
        count++;
    }
}

int main() {
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr *header;
    const unsigned char *data;
    struct PacketNode *packet_list = NULL;
    
    // Open the pcap file
    handle = pcap_open_offline("packets.pcap", errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Error opening pcap file: %s\n", errbuf);
        return 1;
    }

    // Read packets and store them in the linked list
    while (pcap_next_ex(handle, &header, &data) > 0) {
        add_packet(&packet_list, header, data);
    }

    // Print packets
    print_packets(packet_list);

    // Cleanup
    pcap_close(handle);
    return 0;
}
