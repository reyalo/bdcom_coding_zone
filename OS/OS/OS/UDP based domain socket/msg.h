#ifndef __DOMAIN_SOCKET_MSE_H_
#define __DOMAIN_SOCKET_MSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

#define DOMAIN_SOCKET_SERVER_PATH "/tmp/ipc_udp_server.sock"
#define DOMAIN_SOCKET_CLIENT_PATH "/tmp/ipc_udp_client.sock"
#define BUFFER_SIZE 1024
#define FILE_NAME_LEN 32

enum ret_code
{
	RET_CODE_SUCCESS,
	RET_CODE_FAILURE
};

enum tlv_type
{
	TLV_TYPE_FILENAME = 1,
	TLV_TYPE_FILECONTENT,
	TLV_TYPE_FILEERROR
};

typedef struct tlv_data
{
	uint8_t type;
	uint32_t len;
	uint8_t value[0];
} __attribute__((packed)) tlv_data_t;

#endif
