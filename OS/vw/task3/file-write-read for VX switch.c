#include <libfile/file_sys.h>

#define LEN_BUF 64

void file_write(void)
{
	FCB_POINT * fp;
	int len = 0;
	char *buf = "Hello BDCOM, I'm Jerry!";
	
	fp = file_open("test.txt", "w", NULL);
	if (fp == NULL)
	{
		Print("Here is a mistake When open with write the file!\n");
		return;
	}
	
	len = file_write(fp, buf, strlen(buf));
	
	Print("The write length is %d\n", len);

	file_close(fp);
	
	return;
}

void file_read(void)
{
	FCB_POINT * fp;
	char buf[LEN_BUF];
	int len = 0;

    fp = file_open("test.txt", "r", NULL);
	if (fp == NULL)
	{
		Print("Here is a mistake When open with read the file!\n");
		return;
	}
	
	memset(buf, 0, LEN_BUF);
	len = file_read(fp, buf, LEN_BUF - 1);
	
	Print("The read len is %d, data is %s\n", len, buf); 
	
	file_close(fp);
	
	return;
}

