#define _GNU_SOURCE

#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "target.h"

int parse_target(struct target *h, char *data)
{
    char *d;

    h->state = ADB_PROCESS;

    d = strtok(data, ":");
    if(!d)
    {
        return 0;
    }

    h->addr = inet_addr(d);

    d = strtok(NULL, ":");
    if(!d)
    {
        return 0;
    }

    h->port = atoi(d);

    if(!h->port)
    {
        return 0;
    }

    h->port = htons(h->port);

    printf("%d.%d.%d.%d:%d\n", h->addr & 0xff, (h->addr >> 8) & 0xff, (h->addr >> 16) & 0xff, (h->addr >> 24) & 0xff, ntohs(h->port));

    return 1;
}
