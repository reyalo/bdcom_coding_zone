#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_LEN 1024

int main(int argc, char *argv[])
{
	char buf[BUFFER_LEN] = {0};
	
	int fd[2] = {0};
	pid_t pid = 0;
	
    if (pipe(fd) == -1)
	{
		printf("errno = %d: %s\n", errno, strerror(errno));
		return -1;
	}
	
    if((pid = fork()) == -1)
    {
		printf("errno = %d: %s\n", errno, strerror(errno));
		return -2;
	}
	
	if(pid == 0)
	{
		close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
		dup2(fd[1], STDERR_FILENO);
		close(fd[1]);
		
		if (execlp("ping", "ping", "-c", "3", "127.0.0.1", (char*)0) < 0)
			printf("%s\n", strerror(errno));
	}
	
	close(fd[1]);
	waitpid(pid, 0, 0);
	
	if(read(fd[0], buf, BUFFER_LEN) < 0)
	{
		close(fd[0]);
		printf("errno = %d: %s\n", errno, strerror(errno));
		return -3;
	}
		
	close(fd[0]);
	
	printf("%s\n", buf);

	return 0;
}

