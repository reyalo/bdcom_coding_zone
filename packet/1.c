
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  // For inet_pton

#define MAX_LINE_LENGTH 256

// Define a structure for packet data
typedef struct Packet {
    char src_mac[18];
    char dst_mac[18];
    unsigned int src_ip;
    unsigned int dst_ip;
    struct Packet *next;
} Packet;

// Function to create a new packet node
Packet* create_packet(const char *src_mac, const char *dst_mac, const char *src_ip_str, const char *dst_ip_str) {
    Packet *new_packet = (Packet *)malloc(sizeof(Packet));
    if (!new_packet) {
        perror("Failed to allocate memory for new packet");
        exit(EXIT_FAILURE);
    }
    strncpy(new_packet->src_mac, src_mac, sizeof(new_packet->src_mac));
    strncpy(new_packet->dst_mac, dst_mac, sizeof(new_packet->dst_mac));

    inet_pton(AF_INET, src_ip_str, &(new_packet->src_ip));
    inet_pton(AF_INET, dst_ip_str, &(new_packet->dst_ip));
    new_packet->next = NULL;
    return new_packet;
}

// Function to append a packet to the linked list
void append_packet(Packet **head, Packet *new_packet) {
    if (*head == NULL) {
        *head = new_packet;
    } else {
        Packet *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_packet;
    }
}

// Function to process packets from the input file and build the linked list
void process_packet(FILE *input_file, Packet **head) {
    char line[MAX_LINE_LENGTH];
    char src_mac[18], dst_mac[18], src_ip_str[16], dst_ip_str[16];

    while (fgets(line, sizeof(line), input_file)) {
        // Extract MAC addresses
        if (sscanf(line, "Ethernet II, Src: %*[^ ] (%17[^)]), Dst: %*[^ ] (%17[^)])", src_mac, dst_mac) == 2) {
            // Read the next line for IP addresses
            if (fgets(line, sizeof(line), input_file)) {
                if (sscanf(line, "Internet Protocol Version 4, Src: %15[^,], Dst: %15s", src_ip_str, dst_ip_str) == 2) {
                    // Create a new packet and append it to the linked list
                    Packet *new_packet = create_packet(src_mac, dst_mac, src_ip_str, dst_ip_str);
                    append_packet(head, new_packet);
                }
            }
        }
    }
}

// Function to write the linked list to the output file
void write_packets_to_file(FILE *output_file, Packet *head) {
    Packet *current = head;
    char src_ip_str[INET_ADDRSTRLEN], dst_ip_str[INET_ADDRSTRLEN];
    while (current != NULL) {
        inet_ntop(AF_INET, &(current->src_ip), src_ip_str, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(current->dst_ip), dst_ip_str, INET_ADDRSTRLEN);
        fprintf(output_file, "SMAC: %s | DMAC: %s | SIP: %s | DIP: %s\n",
                current->src_mac, current->dst_mac, src_ip_str, dst_ip_str);
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
    FILE *input_file = fopen("packets.txt", "r");
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


/***************************************************************** */



/*
// successfully read mac and ip, stored in structured linked list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

// Define a structure for packet data
typedef struct Packet {
    char src_mac[18];
    char dst_mac[18];
    char src_ip[16];
    char dst_ip[16];
    struct Packet *next;
} Packet;

// Function to create a new packet node
Packet* create_packet(const char *src_mac, const char *dst_mac, const char *src_ip, const char *dst_ip) {
    Packet *new_packet = (Packet *)malloc(sizeof(Packet));
    if (!new_packet) {
        perror("Failed to allocate memory for new packet");
        exit(EXIT_FAILURE);
    }
    strncpy(new_packet->src_mac, src_mac, sizeof(new_packet->src_mac));
    strncpy(new_packet->dst_mac, dst_mac, sizeof(new_packet->dst_mac));
    strncpy(new_packet->src_ip, src_ip, sizeof(new_packet->src_ip));
    strncpy(new_packet->dst_ip, dst_ip, sizeof(new_packet->dst_ip));
    new_packet->next = NULL;
    return new_packet;
}

// Function to append a packet to the linked list
void append_packet(Packet **head, Packet *new_packet) {
    if (*head == NULL) {
        *head = new_packet;
    } else {
        Packet *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_packet;
    }
}

// Function to process packets from the input file and build the linked list
void process_packet(FILE *input_file, Packet **head) {
    char line[MAX_LINE_LENGTH];
    char src_mac[18], dst_mac[18], src_ip[16], dst_ip[16];

    while (fgets(line, sizeof(line), input_file)) {
        // Extract MAC addresses
        if (sscanf(line, "Ethernet II, Src: %*[^ ] (%17[^)]), Dst: %*[^ ] (%17[^)])", src_mac, dst_mac) == 2) {
            // Read the next line for IP addresses
            if (fgets(line, sizeof(line), input_file)) {
                if (sscanf(line, "Internet Protocol Version 4, Src: %15[^,], Dst: %15[^,]", src_ip, dst_ip) == 2) {
                    // Create a new packet and append it to the linked list
                    Packet *new_packet = create_packet(src_mac, dst_mac, src_ip, dst_ip);
                    append_packet(head, new_packet);
                }
            }
        }
    }
}

// Function to write the linked list to the output file
void write_packets_to_file(FILE *output_file, Packet *head) {
    Packet *current = head;
    while (current != NULL) {
        fprintf(output_file, "SMAC: %s | DMAC: %s | SIP: %s | DIP: %s\n",
                current->src_mac, current->dst_mac, current->src_ip, current->dst_ip);
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
    FILE *input_file = fopen("packets.txt", "r");
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





//***************************************************************************************** */




// successfully read mac and ip, but it save in string
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define MAX_LINE_LENGTH 256

// void process_packet(FILE *input_file, FILE *output_file) {
//     char line[MAX_LINE_LENGTH];
//     char src_mac[18], dst_mac[18], src_ip[16], dst_ip[16];

//     while (fgets(line, sizeof(line), input_file)) {
//         // Extract MAC addresses
//         if (sscanf(line, "Ethernet II, Src: %*[^ ] (%17[^)]), Dst: %*[^ ] (%17[^)])", src_mac, dst_mac) == 2) {
//             // Read the next line for IP addresses
//             if (fgets(line, sizeof(line), input_file)) {
//                 if (sscanf(line, "Internet Protocol Version 4, Src: %15[^,], Dst: %15[^,]", src_ip, dst_ip) == 2) {
//                     // Write to output file
//                     fprintf(output_file, "SMAC: %s | DMAC: %s | SIP: %s | DIP: %s\n", src_mac, dst_mac, src_ip, dst_ip);
//                 }
//             }
//         }
//     }
// }

// int main() {
//     FILE *input_file = fopen("packets.txt", "r");
//     if (!input_file) {
//         perror("Error opening input file");
//         return 1;
//     }

//     FILE *output_file = fopen("packets_output.txt", "w");
//     if (!output_file) {
//         perror("Error opening output file");
//         fclose(input_file);
//         return 1;
//     }

//     process_packet(input_file, output_file);

//     fclose(input_file);
//     fclose(output_file);

//     printf("Packets saved to packets_output.txt\n");
//     return 0;
// }








// debugeed code
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define MAX_LINE_LENGTH 256

// void process_packet(FILE *input_file, FILE *output_file) {
//     char line[MAX_LINE_LENGTH];
//     char src_mac[18], dst_mac[18], src_ip[16], dst_ip[16];

//     while (fgets(line, sizeof(line), input_file)) {
//         printf("Read line: %s", line);  // Debugging statement

//         // Extract MAC addresses
//         if (sscanf(line, "Ethernet II, Src: %*[^ ] (%17[^)]), Dst: %*[^ ] (%17[^)])", src_mac, dst_mac) == 2) {
//             printf("Extracted MACs: SMAC: %s, DMAC: %s\n", src_mac, dst_mac);  // Debugging statement

//             // Read the next line for IP addresses
//             if (fgets(line, sizeof(line), input_file)) {
//                 printf("Read line: %s", line);  // Debugging statement

//                 if (sscanf(line, "Internet Protocol Version 4, Src: %15[^,], Dst: %15[^,]", src_ip, dst_ip) == 2) {
//                     printf("Extracted IPs: SIP: %s, DIP: %s\n", src_ip, dst_ip);  // Debugging statement

//                     // Write to output file
//                     fprintf(output_file, "SMAC: %s | DMAC: %s | SIP: %s | DIP: %s\n", src_mac, dst_mac, src_ip, dst_ip);
//                 } else {
//                     printf("Failed to extract IPs from line: %s", line);  // Debugging statement
//                 }
//             } else {
//                 printf("Failed to read line for IP addresses\n");  // Debugging statement
//             }
//         } else {
//             printf("Failed to extract MACs from line: %s", line);  // Debugging statement
//         }
//     }
// }

// int main() {
//     FILE *input_file = fopen("packets.txt", "r");
//     if (!input_file) {
//         perror("Error opening input file");
//         return 1;
//     }

//     FILE *output_file = fopen("packets_output.txt", "w");
//     if (!output_file) {
//         perror("Error opening output file");
//         fclose(input_file);
//         return 1;
//     }

//     process_packet(input_file, output_file);

//     fclose(input_file);
//     fclose(output_file);

//     printf("Packets saved to packets_output.txt\n");
//     return 0;
// }