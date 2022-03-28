#pragma once

#include <fcntl.h>

typedef char BOOL;

static int enable = 1;

#define NONBLOCK(fd) (fcntl(fd, F_SETFL, O_NONBLOCK) | fcntl(fd, F_GETFL, 0))
#define REUSE_ADDR(fd) (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)))

#define GET_VAL(x, data_type) (*(data_type *)x)
#define _OPEN fopen
#define _CLOSE fclose
