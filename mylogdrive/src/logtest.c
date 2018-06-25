#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd;
    int val = 1, count = 0;
    fd = open("/dev/logdev", O_RDWR);
    if(fd < 0){
        printf("can't open!\n");
    }
    while(1)
    {
    	printf("%d\n", count);
    	write(fd, &val, count);
    	count++;
    	sleep(1);
    }
    return 0;
}
