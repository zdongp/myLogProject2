#include <readlog.h>


int main(void)
{
	char *pstr, tmp[15] = {0};

    //open MYLOG
	int fd = open(MYLOG, O_RDWR|O_CREAT, 0777);
	if(fd < 0)
	{
		printf("%s open failed\n", MYLOG);
		return -1;
	}

	//read offset
	read(fd, buf, 15);
	pstr = strtok(buf, ":");
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
		if(klogctl(SYSLOG_ACTION_READ_CLEAR, buf, 1024))
		{		
			if(offset > MAX_SIZE)
			{
				lseek(fd, 0, SEEK_SET);
				write(fd, DEFAULT_STRING, strlen(DEFAULT_STRING));
				offset = DEFAULT_VALUE;
			}

			lseek(fd, offset, SEEK_SET);
			write(fd, buf, strlen(buf));
			printf("%s\n", buf);

			offset += strlen(buf);
			sprintf(tmp, "offset:%04d\n", offset);//unified format
			lseek(fd, 0, SEEK_SET);
			write(fd, tmp, strlen(tmp));
			printf("offset:%04d\n", offset);

			memset(buf, 0, sizeof(buf));
			memset(tmp, 0, sizeof(tmp));

			sleep(3);
		}		
	}


	return 0;
}

