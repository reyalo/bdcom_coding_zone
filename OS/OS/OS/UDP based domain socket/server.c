#include "msg.h"

void get_content_from_specify_file(int *data_size, char **ptr_content)
{
	int fd;
	tlv_data_t *tlv = NULL;
	char file_name[FILE_NAME_LEN] = {0};
	
	if (ptr_content == NULL || data_size == 0)
	{
		printf("Invalid parameters.\n");
		return;
	}
	
	tlv = (tlv_data_t *)*ptr_content;
	if(tlv->type != TLV_TYPE_FILENAME)
	{
		printf("Unmatched packet.\n");
		return;
	}
	
	strncpy(file_name, tlv->value, tlv->len);
	memset(tlv, 0, BUFFER_SIZE);
	
	if ((fd = open(file_name, O_RDONLY)) < 0) 
	{
		tlv->type = TLV_TYPE_FILEERROR;
		sprintf(tlv->value, "filename %s, %s", file_name, strerror(errno));
		tlv->len = strlen(tlv->value);
		*data_size = tlv->len + sizeof(tlv_data_t);
	
		return;
	}
	
	{
		struct stat sb;
		if (stat(file_name, &sb) == -1) 
		{
			printf("Get stat of file failed with %s\n", strerror(errno));
			return;
		}
		
		if (sb.st_size > BUFFER_SIZE)
		{
			*ptr_content = (char *)malloc(sb.st_size + 1);
			if (*ptr_content == NULL)
			{
				printf("Malloc failed when get content of specify file\n");
				return;
			}
		}
		
		tlv = (tlv_data_t *)*ptr_content;
		
		tlv->type = TLV_TYPE_FILECONTENT;
		tlv->len = sb.st_size;
		read(fd, tlv->value, sb.st_size);
		*data_size = tlv->len + sizeof(tlv_data_t);
		
		printf("File size: %d bytes\n", sb.st_size);
	}
	
	close(fd);
	
	return;
}

int main(void) 
{
    int socket_fd = 0, data_size = 0;
	char *ptr_content = NULL;
	socklen_t addrlen;
	char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_un servaddr, cliaddr;
	tlv_data_t *tlv = (tlv_data_t *)buffer;

    if ((socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) 
	{
        printf("Creating socket failed with %s\n", strerror(errno));
        return RET_CODE_FAILURE;
    }
	
	// bind block
	{
		unlink(DOMAIN_SOCKET_SERVER_PATH);
		
		if (access(DOMAIN_SOCKET_SERVER_PATH, 0) != -1)
		{
			remove(DOMAIN_SOCKET_SERVER_PATH);
		}

		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sun_family = AF_UNIX;
		strcpy(servaddr.sun_path, DOMAIN_SOCKET_SERVER_PATH);

		if (bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
		{
			printf("Binding socket failed with %s\n", strerror(errno));
			goto release_resource;
		}
	}

    while (1) 
	{
		memset(&cliaddr, 0, sizeof(cliaddr));
		addrlen = sizeof(cliaddr);
		
		// recv
		{
			memset(buffer, 0, BUFFER_SIZE);
		
			if (recvfrom(socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&cliaddr, &addrlen) < 0)
			{
				printf("Receiving socket failed with %s\n", strerror(errno));
				goto release_resource;
			}
		}
		
		{
			ptr_content = buffer;
			get_content_from_specify_file(&data_size, &ptr_content);
		}
		
		
		// send
		{
			int left_need_to_send = data_size, already_send = 0;
			
			while (left_need_to_send)
			{
				if ((already_send += sendto(socket_fd, 
					ptr_content + already_send, 
					left_need_to_send > BUFFER_SIZE?BUFFER_SIZE:left_need_to_send, 0,
					(struct sockaddr*)&cliaddr, addrlen)) < 0)
				{
					printf("Sending data failed with %s\n", strerror(errno));
					goto release_resource;
				}
				
				left_need_to_send = data_size - already_send;
			}
		}
		
		if (data_size > BUFFER_SIZE)
			free(ptr_content);
    }

release_resource:
    close(socket_fd);

    return RET_CODE_SUCCESS;
}

