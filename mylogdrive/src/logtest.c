#include "logtest.h"

int main(void)
{
    int fd;
    int val = 1, count = 0;
    struct pollfd fds[1];
    char esc[3] = "esc";
    int pipe_fd, ret;
    char cmd_rmfifo[50] = {0};

    fds[0].fd = 0;
    fds[0].events = POLLIN;

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
    pipe_fd = open(FIFO, O_RDWR);
    if(pipe_fd < 0)
    {
        printf("open fifo failed!\n");
        return -1;
    }


    fd = open(LOGDEV, O_RDWR);
    if(fd < 0){
        printf("can't open!\n");
    }
    while(1)
    {
        if(poll(fds, 1,0))
        {
            if(getchar()=='\n')
            {
                sprintf(cmd_rmfifo, "rm -f %s", FIFO);
                system(cmd_rmfifo);
                return 0;
            }
            else
            {
                //send message to fifo to stop readlog
                write(pipe_fd, esc, 3);
                while(getchar() != '\n');
                return 0;
            }
        }

        //call system write to update the log
    	printf("%d\n", count);
    	write(fd, &val, count);
    	count++;
    	sleep(1);
    }
    return 0;
}
