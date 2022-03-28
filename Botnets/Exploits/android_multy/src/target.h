#pragma once

#include <stdint.h>

#include "def.h"

enum
{
    ADB_PROCESS = 0,
    CONNECT_TIMEOUT = 7,
    SEND_TIMEOUT = 10,
    READ_TIMEOUT = 15
};

struct target
{
    int fd;
    uint8_t state;
    uint32_t addr;
    uint16_t port;
    uint32_t timeout;
    int epoll_fd;
    uint32_t timeout_seconds;
    int payload_index;
    int ran;
};

int parse_target(struct target *, char *);
