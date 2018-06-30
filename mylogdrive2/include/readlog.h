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

#define MYLOG 						"mylog.log"
#define	SYSLOG_ACTION_READ_CLEAR 	4
#define FILE_SIZE					1024//1.024e6->1024 In order to observe the effect conveniently
#define BUF_SIZE					1024
#define DEFAULT_VALUE				15
#define DEFAULT_STRING				"offset:0000000\n"


char buf[1024];
int  size;
int  offset = DEFAULT_VALUE;

//root@dong:/home/dong/code/mylogdrive#   (pay attention to authority)
//Kernle related operations must be performed under administrator mode.
//gcc -o ../bin/readlog readlog.c -I../include

#endif