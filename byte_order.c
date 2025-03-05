#include <stdio.h>

#define BUF_LEN 24  // Define buffer length

void main(void) {
    // Data order: smac/dmac/sip/dip/sport/dport
        unsigned char raw_data[BUF_LEN] = {      //used unsigned otherwise 0xa1 showing ffffffa1
        0x98, 0x45, 0x62, 0xd6, 0xa1, 0x6c,      // Source MAC
        0x20, 0x7b, 0xd2, 0x51, 0x19, 0x05,      // Destination MAC
        0x01, 0x02, 0x03, 0x04,                  // Source IP
        0x11, 0x22, 0x33, 0x44,                  // Destination IP
        0x12, 0x34,                              // Source Port
        0x56, 0x78                               // Destination Port
    };

    // Extract and print MAC addresses
    printf("\n\nMAC src=%02x:%02x:%02x:%02x:%02x:%02x, ",
           raw_data[0], raw_data[1], raw_data[2], raw_data[3], raw_data[4], raw_data[5]);

    printf("dst=%02x:%02x:%02x:%02x:%02x:%02x ",
           raw_data[6], raw_data[7], raw_data[8], raw_data[9], raw_data[10], raw_data[11]);

    // Extract and print IP addresses
    printf("IP src=%d.%d.%d.%d, ",
           raw_data[12], raw_data[13], raw_data[14], raw_data[15]);

    printf("dst=%d.%d.%d.%d, ",
           raw_data[16], raw_data[17], raw_data[18], raw_data[19]);

    // Extract and print TCP ports (convert from big-endian, show in hex)
    unsigned short sport = (raw_data[20] << 8) | raw_data[21];
    unsigned short dport = (raw_data[22] << 8) | raw_data[23];

    printf("TCP sport=%04X, dport=%04X\n", sport, dport);

    
}
