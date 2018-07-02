
#include <sys/klog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define MYLOG 						"mylog.log"
#define	SYSLOG_ACTION_READ_CLEAR 	4
#define FILE_SIZE					1024*5//1.024e6->1024 In order to observe the effect conveniently
#define BUF_SIZE					1024
#define DEFAULT_VALUE				15
#define DEFAULT_STRING				"offset:0000000\n"


//root@dong:/home/dong/code/mylogdrive#   (pay attention to authority)
//Kernle related operations must be performed under administrator mode.
//gcc -o ../bin/readlog readlog.c -I../include

int main(void)
{
	
	char *pstr; 
	int  offset = DEFAULT_VALUE;
	char buf_log[BUF_SIZE] = {0};
	char buf_offset[BUF_SIZE] = {0};

    //open MYLOG
	int fd = open(MYLOG, O_RDWR|O_CREAT, 0777);
	if(fd < 0)
	{
		printf("%s open failed\n", MYLOG);
		return -1;
	}

	//read offset
	read(fd, buf_log, DEFAULT_VALUE);
	pstr = strtok(buf_log, ":");
	if(pstr != NULL)
	{	
		pstr = strtok(NULL, ":");
		offset = atoi(pstr);
		printf("offset:%d\n", atoi(pstr));	
	}
	else
		write(fd, DEFAULT_STRING, strlen(DEFAULT_STRING));


	while(1)
	{
		//read ring_buf
		if(klogctl(SYSLOG_ACTION_READ_CLEAR, buf_log, BUF_SIZE))
		{		
			if(offset > FILE_SIZE)
			{
				lseek(fd, 0, SEEK_SET);
				write(fd, DEFAULT_STRING, strlen(DEFAULT_STRING));
				offset = DEFAULT_VALUE;
			}

			lseek(fd, offset, SEEK_SET);
			write(fd, buf_log, strlen(buf_log));
			printf("%s\n", buf_log);

			offset += strlen(buf_log);
			sprintf(buf_offset, "offset:%07d\n", offset);//unified format
			lseek(fd, 0, SEEK_SET);
			write(fd, buf_offset, strlen(buf_offset));
			printf("offset:%07d\n", offset);

			memset(buf_log, 0, sizeof(buf_log));
			memset(buf_offset, 0, sizeof(buf_offset));

			sleep(2);
		}		
	}

	close(fd);

	return 0;
}

