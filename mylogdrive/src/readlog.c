#include <sys/klog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>


#define MYLOG 						"./mylog.log"
#define	SYSLOG_ACTION_READ_CLEAR 	4
#define MAX_SIZE					1024//1.024e6->1024 In order to observe the effect conveniently
#define DEFAULT_VALUE				8
#define DEFAULT_STRING				"offset:8\n"


char buf[1024];
int  size;
int  offset;

//root@dong:/home/dong/code/mylogdrive#   (pay attention to authority)
//Kernle related operations must be performed under administrator mode.


int main(void)
{
	char *pstr, tmp[15] = {0};
	//There is difference in the order of fopen and open here.
	
	int fd = open(MYLOG, O_RDWR|O_CREAT, 0777);
	FILE* fp = fdopen(fd, "r");
	if(fd < 0 && fp !=NULL)
	{
		printf("%s open failed\n", MYLOG);
		return -1;
	}

	read(fd, buf, 15);
	pstr = strtok(buf, ":");
	if(pstr != NULL)
	{	
		pstr = strtok(NULL, ":");
		offset = atoi(pstr);
		printf("offset:%d\n", atoi(pstr));	
	}
	else
		write(fd, DEFAULT_STRING, sizeof(DEFAULT_STRING));


	while(1)
	{
		if(klogctl(SYSLOG_ACTION_READ_CLEAR, buf, 1024))
		{		
			if(offset > MAX_SIZE)
			{
				lseek(fd, 0, SEEK_SET);
				write(fd, DEFAULT_STRING, sizeof(DEFAULT_STRING));
				offset = DEFAULT_VALUE;
			}

			lseek(fd, offset, SEEK_SET);
			write(fd, buf, strlen(buf));
			printf("%s\n", buf);

			offset += strlen(buf);
			sprintf(tmp, "offset:%d\n", offset);
			lseek(fd, 0, SEEK_SET);
			write(fd, tmp, strlen(tmp));
			printf("offset:%d\n", offset);

			memset(buf, 0, sizeof(buf));
			memset(tmp, 0, sizeof(tmp));
			//The appropriate delay is important for data acquisition
			sleep(3);
		}		
	}


	return 0;
}

