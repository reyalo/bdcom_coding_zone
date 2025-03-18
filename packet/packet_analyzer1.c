#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ether.h>
#include <arpa/inet.h>

// Define the structure to store packets
struct PacketNode {
    struct in_addr src_ip;
    struct in_addr dest_ip;
    unsigned char src_mac[6];
    unsigned char dest_mac[6];
    int count;
    struct PacketNode *next;
};

// Function to print MAC address
void print_mac(const unsigned char *mac) {
    printf("%02X:%02X:%02X:%02X:%02X:%02X", 
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// Function to find or create a packet node
struct PacketNode *find_or_create(struct PacketNode **head, 
                                  struct in_addr src, struct in_addr dest,
                                  unsigned char *smac, unsigned char *dmac) {
    struct PacketNode *current = *head;

    // Search for an existing entry
    while (current) {
        if (current->src_ip.s_addr == src.s_addr && 
            current->dest_ip.s_addr == dest.s_addr &&
            memcmp(current->src_mac, smac, 6) == 0 &&
            memcmp(current->dest_mac, dmac, 6) == 0) {
            current->count++;  // Increment reference count
            return current;
        }
        current = current->next;
    }

    // If not found, create a new node
    struct PacketNode *new_node = (struct PacketNode *)malloc(sizeof(struct PacketNode));
    new_node->src_ip = src;
    new_node->dest_ip = dest;
    memcpy(new_node->src_mac, smac, 6);
    memcpy(new_node->dest_mac, dmac, 6);
    new_node->count = 1;
    new_node->next = *head;
    *head = new_node;

    return new_node;
}

// Function to process a captured packet
void process_packet(struct PacketNode **head, 
                    const struct pcap_pkthdr *pkt_header, const unsigned char *pkt_data) {
    struct ether_header *eth_header = (struct ether_header *)pkt_data;

    // Check if it's an IP packet
    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        struct ip *ip_header = (struct ip *)(pkt_data + sizeof(struct ether_header));

        // Print packet information
        printf("\nPacket Info:\n");
        printf("SMAC: ");
        print_mac(eth_header->ether_shost);
        printf(" | DMAC: ");
        print_mac(eth_header->ether_dhost);
        printf(" | SIP: %s | DIP: %s\n", 
               inet_ntoa(ip_header->ip_src), inet_ntoa(ip_header->ip_dst));

        // Store packet information
        find_or_create(head, ip_header->ip_src, ip_header->ip_dst, 
                       eth_header->ether_shost, eth_header->ether_dhost);
    }
}

// Function to print packet summary
void print_summary(struct PacketNode *head) {
    printf("\nPacket Summary:\n");
    printf("------------------------------------------------------------\n");
    printf("Source MAC       Dest MAC        Source IP    Dest IP      Count\n");
    printf("------------------------------------------------------------\n");

    struct PacketNode *current = head;
    while (current) {
        print_mac(current->src_mac);
        printf(" -> ");
        print_mac(current->dest_mac);
        printf(" | %s -> %s | %d\n",
               inet_ntoa(current->src_ip), inet_ntoa(current->dest_ip),
               current->count);
        current = current->next;
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

    // Read packets and process them
    while (pcap_next_ex(handle, &header, &data) > 0) {
        process_packet(&packet_list, header, data);
    }

    // Print summary
    print_summary(packet_list);

    // Cleanup
    pcap_close(handle);
    return 0;
}
