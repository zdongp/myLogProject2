#include <readlog.h>


int main(void)
{
	char *pstr, tmp[15] = {0};
    int pipe_fd, ret;
    char esc[3];
    char cmd_rmfifo[50] = {0};
    //create fifo
    if(access(FIFO, F_OK)==-1)
    {
        ret = mkfifo(FIFO, 0777);
        if(ret!=0)
        {
            printf("can't create fifo\n");
            return -1;
        }
    } 
    //open fifo
    pipe_fd = open(FIFO, O_RDWR|O_NONBLOCK);
    if(pipe_fd < 0)
    {
        printf("open fifo failed!\n");
        return -1;
    }


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
		//read fifo nonblock
		read(pipe_fd, tmp, 3);
		if(strcmp(tmp, "esc")==0)
		{
			sprintf(cmd_rmfifo, "rm -f %s", FIFO);
			system(cmd_rmfifo);
			return 0;
		}

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

			//The appropriate delay is important for data acquisition
			sleep(3);
		}		
	}


	return 0;
}

