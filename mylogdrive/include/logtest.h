#ifndef _LOGTEST_H
#define _LOGTEST_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define FIFO   "fifo"
#define LOGDEV "/dev/logdev"
//gcc -o ../bin/logtest logtest.c -I../include

#endif