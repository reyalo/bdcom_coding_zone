#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ether.h>
#include <netinet/ip.h>

// Define the linked list node structure
struct PacketNode {
    char src_ip[16];
    char dest_ip[16];
    int ref_count;
    struct PacketNode *next;
};

// Head of the linked list
struct PacketNode *head = NULL;

// Function to check if a packet exists in the list
struct PacketNode* find_or_add_packet(char *src, char *dest) {
    struct PacketNode *current = head;
    
    // Search for existing entry
    while (current != NULL) {
        if (strcmp(current->src_ip, src) == 0 && strcmp(current->dest_ip, dest) == 0) {
            current->ref_count++;
            return current;
        }
        current = current->next;
    }

    // If not found, create a new node
    struct PacketNode *new_node = (struct PacketNode*)malloc(sizeof(struct PacketNode));
    strcpy(new_node->src_ip, src);
    strcpy(new_node->dest_ip, dest);
    new_node->ref_count = 1;
    new_node->next = head;
    head = new_node;
    return new_node;
}

// Packet handler function
void packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    struct ether_header *eth = (struct ether_header*) bytes;
    struct iphdr *ip = (struct iphdr*) (bytes + sizeof(struct ether_header));

    // Convert IP addresses
    char src_ip[16], dest_ip[16];
    sprintf(src_ip, "%d.%d.%d.%d", ip->saddr & 0xFF, (ip->saddr >> 8) & 0xFF, (ip->saddr >> 16) & 0xFF, (ip->saddr >> 24) & 0xFF);
    sprintf(dest_ip, "%d.%d.%d.%d", ip->daddr & 0xFF, (ip->daddr >> 8) & 0xFF, (ip->daddr >> 16) & 0xFF, (ip->daddr >> 24) & 0xFF);

    // Check if packet already exists
    struct PacketNode *node = find_or_add_packet(src_ip, dest_ip);

    // Print packet information
    printf("SMAC: %s | DMAC: %s | SIP: %s | DIP: %s | Count: %d\n",
           ether_ntoa((struct ether_addr*)eth->ether_shost),
           ether_ntoa((struct ether_addr*)eth->ether_dhost),
           src_ip, dest_ip, node->ref_count);
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline("packets.pcap", errbuf);
    
    if (handle == NULL) {
        printf("Error: %s\n", errbuf);
        return 1;
    }

    // Process packets
    pcap_loop(handle, 0, packet_handler, NULL);
    pcap_close(handle);

    return 0;
}
