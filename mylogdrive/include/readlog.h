#ifndef _READLOG_H
#define _READLOG_H

#include <sys/klog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define FIFO 						"fifo"
#define MYLOG 						"/home/dong/code/mylogdrive/bin/mylog.log"
#define	SYSLOG_ACTION_READ_CLEAR 	4
#define MAX_SIZE					1024//1.024e6->1024 In order to observe the effect conveniently
#define DEFAULT_VALUE				12
#define DEFAULT_STRING				"offset:1024\n"


char buf[1024];
int  size;
int  offset = DEFAULT_VALUE;

//root@dong:/home/dong/code/mylogdrive#   (pay attention to authority)
//Kernle related operations must be performed under administrator mode.
//gcc -o ../bin/readlog readlog.c -I../include

#endif