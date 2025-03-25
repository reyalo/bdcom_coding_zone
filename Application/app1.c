#include<stdio.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_LINE 256
#define MAX_BYTES 1500


typedef struct {

    #if __BYTE_ORDER == __LITTLE_ENDIAN
    uint32_t reserve1:1;
    uint32_t port_rate:3;
    uint32_t port_negotiation_mode:2;
    uint32_t port_duplex_mode:2;

    uint32_t port_inlet_speed_limit:7;
    uint32_t port_enable:1;

    uint32_t port_outlet_speed_limit:7;
    uint32_t port_link_state:1;

    uint32_t reserve2:8;


    #elif __BYTE_ORDER == __BIG_ENDIAN

    uint32_t port_duplex_mode:2;
    uint32_t port_negotiation_mode:2;
    uint32_t port_rate:3;
    uint32_t reserve1:1;

    uint32_t port_enable:1;
    uint32_t port_inlet_speed_limit:7;
    
    uint32_t port_link_state:1;
    uint32_t port_outlet_speed_limit:7;

    uint32_t reserve2:8;

    #endif
    
} __attribute__((packed)) port_t;




int main(){

    char buf[4] = {0};
    port_t *pport = (port_t *)buf;

    /*Settin to the value in Table 2*/

    pport->port_duplex_mode = 0b11;
    pport->port_negotiation_mode = 0b01;
    pport->port_rate = 0b011;
    pport->port_enable = 0b1;
    pport->port_inlet_speed_limit = 36;
    pport->port_link_state = 0b1;
    pport->port_outlet_speed_limit = 63;


    printf("%hhx, %hhx, %hhx, %hhx\n", buf[0], buf[1], buf[2], buf[3]);




}