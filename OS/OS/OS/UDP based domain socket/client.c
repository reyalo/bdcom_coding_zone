#include "msg.h"

int main(int argc, char *argv[]) 
{
    int socket_fd;
	socklen_t addrlen;
	char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_un servaddr, cliaddr;
	tlv_data_t *tlv = (tlv_data_t *)buffer;
	
	if (!(argc == 2 && argv[1]))
	{
		printf("Pls input the filename\n");
		return RET_CODE_FAILURE;
	}

    if ((socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) 
	{
        printf("Creating socket failed with %s\n", strerror(errno));
        return RET_CODE_FAILURE;
    }
	
	// bind block
	{
		if (access(DOMAIN_SOCKET_CLIENT_PATH, 0) != -1)
		{
			remove(DOMAIN_SOCKET_CLIENT_PATH);
		}
		
		memset(&cliaddr, 0, sizeof(cliaddr));
		cliaddr.sun_family = AF_UNIX;
		strcpy(cliaddr.sun_path, DOMAIN_SOCKET_CLIENT_PATH);
		
		if (bind(socket_fd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0) 
		{
			printf("Binding socket failed with %s\n", strerror(errno));
			goto release_resource;
		}
	}
	
	// for send & recv
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, DOMAIN_SOCKET_SERVER_PATH);
	addrlen = sizeof(servaddr);
	
	{
		tlv->type = TLV_TYPE_FILENAME;
		tlv->len = strlen(argv[1]);
		memcpy(tlv->value, argv[1], tlv->len);
		if (sendto(socket_fd, buffer, tlv->len + sizeof(tlv_data_t), 0,(struct sockaddr*)&servaddr, addrlen) < 0)
		{
			printf("Sending data failed with %s\n", strerror(errno));
			goto release_resource;
		}
	}
	
	{
		int size = 0, i;
		
		memset(buffer, 0, BUFFER_SIZE);
		while ((size = recvfrom(socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&servaddr, &addrlen)) >= 0)
		{
			for (i = 0; i < size; i++)
				printf("%c", buffer[i]);
			
			if (size < BUFFER_SIZE)
				break;
		}
	}

release_resource:
    close(socket_fd);
	unlink(DOMAIN_SOCKET_CLIENT_PATH);

    return RET_CODE_SUCCESS;
}